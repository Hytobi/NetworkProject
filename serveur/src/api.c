/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>
#include "api.h"
#include "struct.h"
#include "err.h"
#include "game.h"
#include "json.h"
#include "bombe.h"

#define ENVOI_MESSAGE(x, len)                                                                       \
    sprintf(buffer2, "%s", x);                                                                      \
    n = sendto(cl->socket, buffer2, len, MSG_CONFIRM, (struct sockaddr *)&cl->addr, clientAddrLen); \
    if (n == ERR)                                                                                   \
    {                                                                                               \
        perror("Erreur envoie du message");                                                         \
        return;                                                                                     \
    }

#define ENVOIE_ERR_INCONNUE ENVOI_MESSAGE(cJSON_Print(errInconnue()), strlen(cJSON_Print(errInconnue())))
#define ENVOIE_BAD_REQUEST ENVOI_MESSAGE(cJSON_Print(badRequest()), strlen(cJSON_Print(badRequest())))

#define GET_MAP(m, i)                                                         \
    m = getMap(clientMap->mapInfo, clientMap->gameInfo->gameListe[i]->mapId); \
    if (m == NULL)                                                            \
    {                                                                         \
    }

/**
 * Analyse la chaine reçu et renvoie la réponse approprié
 * @param clientMap liaison entre les maps, clients et games
 * @param recu chaine reçu par le client
 */
void receiveSend(Client_Map_Games *clientMap, char *recu) {
    int n = 0;
    Client *cl = clientMap->cl;                 // structure contenant toutes les infos necessaire
    socklen_t clientAddrLen = sizeof(cl->addr); // adresse du Client
    char buffer2[BUFFER_SIZE];

    if (!strcmp(recu, messageClientAttendue)) {
        // Client qui cherche les serveurs
        ENVOI_MESSAGE(notifClientServeurUp, strlen(notifClientServeurUp));
    } else if (!strncmp(recu, postPlayerMove, POST_PLAYER_MOVE_SIZE)) {
        // Client veut se déplacer
        recu += POST_PLAYER_MOVE_SIZE;
        printf("Tentative de déplacement du joueur %s\n", inet_ntoa(cl->addr.sin_addr));

        // déplacement du joueur en bloquant les mutex nécessaires
        pthread_mutex_lock(&cl->clientGame->mutex);
        pthread_mutex_lock(&cl->clientGame->map->mutex);
        //afficheMap(*cl->clientGame->map);
        int caseMove = movePlayer(cl->player, cl->clientGame, cJSON_Parse(recu));
        afficheMap(*cl->clientGame->map);
        pthread_mutex_unlock(&cl->clientGame->map->mutex);
        pthread_mutex_unlock(&cl->clientGame->mutex);

        // Si erreur, on s'arrete
        if (caseMove == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }

        // on créer la chaine à envoyer
        char postMove[BUFFER_SIZE];
        sprintf(postMove, "%s\n%sEOJ", POST_POSITION_PLAYER_UPDATE, cJSON_Print(
                sendMove(cl->player, cJSON_GetObjectItemCaseSensitive(cJSON_Parse(recu), "move")->valuestring)));

        pthread_mutex_lock(&cl->clientGame->mutex);

        // Envoie de la chaine a tout le monde
        for (int i = 0; i < MAX_PLAYER; i++) {
            Player *player = cl->clientGame->players[i];
            if (player == NULL) {
                continue;
            }
            clientAddrLen = sizeof(player->addr);
            n = (int) sendto(player->socket, postMove, strlen(postMove), MSG_CONFIRM,
                             (struct sockaddr *) &player->addr,
                             clientAddrLen);
            if (n == ERR) {
                perror("Erreur envoie du message");
                return;
            }
        }
        pthread_mutex_unlock(&cl->clientGame->mutex);
        printf("Mouvement Réussie !\n");
    } else if (!strncmp(recu, getMapListe, GET_MAP_LISTE_SIZE)) {
        // Client veut la liste des maps par défaut
        printf("Envoie des informations concernant les Maps à : %s ...\n", inet_ntoa(cl->addr.sin_addr));
        char *json = cJSON_Print(sendMapListe(clientMap->mapInfo));
        ENVOI_MESSAGE(json, strlen(json));
        free(json);
        printf("Map envoyé avec Succès\n");
    } else if (!strncmp(recu, postCreateGame, POST_CREATE_GAME_SIZE)) {
        // Client veut créer une game
        recu += POST_CREATE_GAME_SIZE;
        printf("Demande de création de game: %s\n", recu);
        printf("Création de la partie...\n");

        //On créer la game
        pthread_mutex_lock(&clientMap->gameInfo->mutex);
        int indiceGame = createGame(clientMap->gameInfo, clientMap->mapInfo, cJSON_Parse(recu), cl);
        pthread_mutex_unlock(&clientMap->gameInfo->mutex);

        //Si la game n'est pas créer on arrete
        if (indiceGame == ERR) {
            printf("erreur lors de la création de la game\n");
            ENVOIE_ERR_INCONNUE
            return;
        }

        // on fait rejoindre le client dans la game
        Map *m;
        GET_MAP(m, indiceGame);
        pthread_mutex_lock(&clientMap->gameInfo->gameListe[indiceGame]->mutex);
        if (joinGame(clientMap->gameInfo->gameListe[indiceGame], cl, m) == ERR) {
            printf("Erreur join game\n");
            ENVOIE_ERR_INCONNUE;
            pthread_mutex_unlock(&clientMap->gameInfo->gameListe[indiceGame]->mutex);
            return;
        }

        // On créer le message et l'envoie au client
        pthread_mutex_unlock(&clientMap->gameInfo->gameListe[indiceGame]->mutex);
        char *json = cJSON_Print(sendGameCreation(cl->clientGame,
                                                  clientMap->mapInfo->mapListe[cJSON_GetObjectItemCaseSensitive(
                                                          cJSON_Parse(recu), "mapId")
                                                          ->valueint]));
        ENVOI_MESSAGE(json, strlen(json));
        free(json);
        printf("Partie créer !\n");
    } else if (!strncmp(recu, getPartieListe, GET_PARTIE_LISTE_SIZE)) {
        // Demande de la lsite des parties
        printf("Envoie de la liste des parties...\n");

        // création du message contenant la liste des aprties
        pthread_mutex_lock(&clientMap->gameInfo->mutex);
        char *json = cJSON_Print(sendPartieListe(clientMap->gameInfo));
        pthread_mutex_unlock(&clientMap->gameInfo->mutex);

        // envoie de la lsite des parties au client
        ENVOI_MESSAGE(json, strlen(json));
        free(json);
        printf("Envoie efectué !\n");
    } else if (!strncmp(recu, postJoinGame, POST_JOIN_GAME_SIZE)) {
        // Client veut rejoindre une game
        recu += POST_JOIN_GAME_SIZE;
        Game *g;
        int i = 0;

        // On cherche quel game il veut rejoindre
        char name[STRING_SIZE];
        printf("%s Veut rejoindre une game...\n", inet_ntoa(cl->addr.sin_addr));
        strcpy(name, cJSON_GetObjectItemCaseSensitive(cJSON_Parse(recu), "name")->valuestring);
        pthread_mutex_lock(&clientMap->gameInfo->mutex);

        // On essaye de le faire rejoindre la game s'il y a encore de la place
        while (i < MAX_GAMES) {
            if (clientMap->gameInfo->gameListe[i] == NULL) {
                i++;
                continue;
            }
            pthread_mutex_lock(&clientMap->gameInfo->gameListe[i]->mutex);
            if (!strcmp(clientMap->gameInfo->gameListe[i]->name, name)) {
                g = clientMap->gameInfo->gameListe[i];
                break;
            }
            pthread_mutex_unlock(&clientMap->gameInfo->gameListe[i]->mutex);
            i++;
        }
        pthread_mutex_unlock(&clientMap->gameInfo->mutex);
        Map *m = g->map;
        int playerIndex = joinGame(g, cl, m);
        pthread_mutex_unlock(&g->mutex);

        // Si ca se passe mal, on arrete et il ne la rejoint pas
        if (playerIndex == ERR) {
            printf("Erreur joinGame\n");
            ENVOIE_ERR_INCONNUE;
            return;
        }

        // on envoie au client qu'il l'a rejoint et ses infos
        char joinGame[BUFFER_SIZE];
        pthread_mutex_lock(&g->mutex);
        sprintf(joinGame, "%s", cJSON_Print(sendJoinGame(g, g->players[playerIndex])));
        pthread_mutex_unlock(&g->mutex);
        ENVOI_MESSAGE(joinGame, strlen(joinGame));

        // oOn informe tous les joueurs de la partie qu'il y a un nouveau joueur
        pthread_mutex_lock(&g->mutex);
        for (int i = 0; i < MAX_PLAYER; i++) {
            char newP[BUFFER_SIZE];
            sprintf(newP, "%s\n%s", POST_NEW_PLAYER, cJSON_Print(newPlayer(*cl->player)));
            Player *p = g->players[i];
            if (p && p != cl->player) {
                clientAddrLen = sizeof(p->addr);
                n = (int) sendto(p->socket, newP, strlen(newP), MSG_CONFIRM, (struct sockaddr *) &p->addr,
                                 clientAddrLen);
                if (n == ERR) {
                    perror("Erreur envoi message");
                    i--;
                }
            }
        }
        pthread_mutex_unlock(&g->mutex);

        printf("Partie Rejoint avec succès !\n");
    } else if (!strncmp(recu, postPlayerAttack, POST_PLAYER_ATTACK_SIZE)) {
        // Client veut poser une bombe/mine/remote bombe
        recu += POST_PLAYER_ATTACK_SIZE;
        printf("Tentative d'attaque du joueur %s\n", inet_ntoa(cl->addr.sin_addr));
        // On essaye de le faire attaquer
        if (attackPlayer(cl->player, cl->clientGame, cJSON_Parse(recu)) == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }
        char postAttack[BUFFER_SIZE];
        sprintf(postAttack, "%sEOJ", cJSON_Print(sendPosBomb(cJSON_Parse(recu), cl->player)));
        // On lui indique qu'il a attaque
        ENVOI_MESSAGE(postAttack, strlen(postAttack));

        // On envoie a tout le monde la position a mettre a jour avec quelle bombe
        pthread_mutex_lock(&cl->clientGame->mutex);
        for (int i = 0; i < MAX_PLAYER; i++) {
            Player *player = cl->clientGame->players[i];
            if (player == NULL) {
                continue;
            }
            char *postAll = malloc(sizeof(char) * BUFFER_SIZE);
            sprintf(postAll, "%s\n%sEOJ", postAttackNewbomb, cJSON_Print(cJSON_Parse(recu)));
            clientAddrLen = sizeof(player->addr);
            n = (int) sendto(player->socket, postAll, strlen(postAll), MSG_CONFIRM,
                             (struct sockaddr *) &player->addr,
                             clientAddrLen);
            if (n == ERR) {
                perror("Erreur envoie du message");
                return;
            }
            free(postAll);
        }

        // Si c'est une bombe classique on gere un peu autrement, il faut lancer un thread pour le timer
        if (!strncmp(cJSON_GetObjectItemCaseSensitive(cJSON_Parse(recu), "type")->valuestring, CLASSIC, CLASSIC_SIZE)) {

            // On créer la bombe classqiue
            int bombeID = createBombe(cl->clientGame->bombesListe, cl->player);
            if (bombeID == ERR) {
                printf("Erreur lors de la pose de la bombe\n");
                return;
            }

            // On lance le thread d'explosion
            pthread_t bombeThread;
            pthread_create(&bombeThread, NULL, bombeThreadExplose, (void *) cl->clientGame);

            // pthread_join(bombeThread, NULL);
        }

        printf("Bombe posée !\n");

        pthread_mutex_unlock(&cl->clientGame->mutex);
    } else if (!strncmp(recu, postAttackRemoteGo, POST_ATTACK_REMOTE_GO_SIZE)) {
        // Client veut faire exploser ses remotes bombes
        printf("Explosion des remotes bomb de %s\n", inet_ntoa(cl->addr.sin_addr));

        // On essaye de faire exploser les remotes bombes
        pthread_mutex_lock(&cl->clientGame->mutex);
        pthread_mutex_lock(&cl->clientGame->map->mutex);
        int exploseBombeRetour = exploseBomb(cl->clientGame, cl->player);
        pthread_mutex_unlock(&cl->clientGame->map->mutex);
        pthread_mutex_unlock(&cl->clientGame->mutex);

        if (exploseBombeRetour == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }
        printf("Remote bombes explosées !\n");
    } else if (!strncmp(recu, postObjectNew, POST_OBJECT_NEW_SIZE)) {
        printf("Mise à jour du joueur %s\n", inet_ntoa(cl->addr.sin_addr));
        recu += POST_OBJECT_NEW_SIZE;
        if (updatePlayer(cl->player, cJSON_Parse(recu)) == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }
        char postObject[BUFFER_SIZE];
        sprintf(postObject, "%s", cJSON_Print(sendObjNew(cl->player)));
        ENVOI_MESSAGE(postObject, strlen(postObject));
    }
}

/**
 * Fonction a lacner dans un thread permettant d'ecouter et d'envoyer des messages à chaque client
 * @param args
 * @return
 */
void *clientCommunication(void *args) {
    // Structure pour que les clients aient toutes infos dont ils ont besoin
    Client_Map_Games *cm = (Client_Map_Games *) args;
    Client *cl = cm->cl;
    char buffer[BUFFER_SIZE];
    int n = 0;

    //boucle principale du client
    for (;;) {
        // lecture du message
        strcpy(buffer, "");
        // n = recvfrom(cl->socket, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &cl->addr, &clientAddrLen);
        n = (int) read(cl->socket, buffer, BUFFER_SIZE);

        // si erreur
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;

        // Si le client s'est déconnecter
        } else if (n == 0) {
            printf("Connexion à %s perdu !\n", inet_ntoa(cl->addr.sin_addr));

            // Si il etait dans une game
            if (cl->clientGame != NULL) {
                pthread_mutex_lock(&cl->clientGame->mutex);
                int inGame = 0;

                // Analyse que le client était la dernière personne de la game, si oui on détruit la game
                for (int i = 0; i < MAX_PLAYER; i++) {
                    if (cl->clientGame->players[i] != NULL) {
                        if (cl->clientGame->players[i]->socket != cl->socket) {

                            pthread_mutex_unlock(&cl->clientGame->mutex);
                            for (int j = 0; j < MAX_GAMES; j++) {

                                pthread_mutex_lock(&cm->gameInfo->mutex);
                                if (cm->gameInfo->gameListe[j] == cl->clientGame) {
                                    inGame = j;
                                }
                            }
                            break;
                        }
                    }
                }

                // destruction de la game si plus personne n'est présent
                if (!inGame) {
                    printf("Destruction de la partie...\n");
                    destroyGame(cl->clientGame);
                    cm->gameInfo->gameListe[inGame] = NULL;
                    cm->gameInfo->nbGames--;
                    printf("Partie détruite avec succès\n");
                    pthread_mutex_unlock(&cl->clientGame->mutex);
                    pthread_mutex_unlock(&cm->gameInfo->mutex);
                }
            }

            // on ferme le socket
            close(cl->socket);
            if (cm!=NULL) {
                free(cm);
            }

            return NULL;
        }
        buffer[n] = '\0';

        printf("%s : %s\n", inet_ntoa(cl->addr.sin_addr), buffer);

        // Le message à bien été recu, on l'analyse et agit en conséquence
        receiveSend(cm, buffer);
    }

    // noramlement on n'arrive jamais ici
    close(cl->socket);
    return NULL;
}