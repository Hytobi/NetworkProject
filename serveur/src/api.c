//
// Created by hana on 04/12/23.
//

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

#define ENVOI_MESSAGE(x, len) sprintf(buffer2, "%s", x); \
n = sendto(cl->socket, buffer2, len, MSG_CONFIRM, (struct sockaddr *) &cl->addr, clientAddrLen); \
if (n == ERR) { \
perror("Erreur envoie du message"); \
return; \
}

#define ENVOIE_ERR_INCONNUE ENVOI_MESSAGE(cJSON_Print(errInconnue()), strlen(cJSON_Print(errInconnue())))
#define ENVOIE_BAD_REQUEST ENVOI_MESSAGE(cJSON_Print(badRequest()), strlen(cJSON_Print(badRequest())))

#define GET_MAP(m, i) m = getMap(clientMap->mapInfo,clientMap->gameInfo->gameListe[i]->mapId);\
if (m==NULL){ \
 \
}

/**
 * Analyse la chaine reçu et renvoie la réponse approprié
 * @param recu
 */
void receiveSend(Client_Map_Games *clientMap, char *recu) {
    int n = 0;
    Client *cl = clientMap->cl; // structure contenant toutes les infos necessaire
    socklen_t clientAddrLen = sizeof(cl->addr); // adresse du Client
    char buffer2[BUFFER_SIZE];
    if (!strcmp(recu, messageClientAttendue)) {
        ENVOI_MESSAGE(notifClientServeurUp, strlen(notifClientServeurUp));
    } else if (!strncmp(recu, getMapListe, GET_MAP_LISTE_SIZE)) {
        printf("Envoie des informations concernant les Maps à : %s ...\n", inet_ntoa(cl->addr.sin_addr));
        ENVOI_MESSAGE(cJSON_Print(sendMapListe(clientMap->mapInfo)),
                      strlen(cJSON_Print(sendMapListe(clientMap->mapInfo))));
        printf("Map envoyé avec Succès\n");
    } else if (!strncmp(recu, postCreateGame, POST_CREATE_GAME_SIZE)) {
        recu += POST_CREATE_GAME_SIZE;
        printf("Demande de création de game: %s\n", recu);
        printf("Création de la partie...\n");
        pthread_mutex_lock(&clientMap->gameInfo->mutex);
        int indiceGame = createGame(clientMap->gameInfo, clientMap->mapInfo, cJSON_Parse(recu), cl);
        pthread_mutex_unlock(&clientMap->gameInfo->mutex);
        if (indiceGame == ERR) {
            printf("erreur lors de la création de la game\n");
            ENVOIE_ERR_INCONNUE
            return;
        }
        Map *m;
        GET_MAP(m, indiceGame);
        pthread_mutex_lock(&clientMap->gameInfo->gameListe[indiceGame]->mutex);
        if (joinGame(clientMap->gameInfo->gameListe[indiceGame], cl, m) == ERR) {
            printf("Erreur join game\n");
            ENVOIE_ERR_INCONNUE;
            pthread_mutex_unlock(&clientMap->gameInfo->gameListe[indiceGame]->mutex);
            return;
        }
        pthread_mutex_unlock(&clientMap->gameInfo->gameListe[indiceGame]->mutex);
        char *json = cJSON_Print(sendGameCreation(cl->clientGame,
                                                  clientMap->mapInfo->mapListe[cJSON_GetObjectItemCaseSensitive(
                                                          cJSON_Parse(recu), "mapId")->valueint]));
        ENVOI_MESSAGE(json, strlen(json));
        printf("Partie créer !\n");
    } else if (!strncmp(recu, getPartieListe, GET_PARTIE_LISTE_SIZE)) {
        printf("Envoie de la liste des parties...\n");
        ENVOI_MESSAGE(cJSON_Print(sendPartieListe(clientMap->gameInfo)),
                      strlen(cJSON_Print(sendPartieListe(clientMap->gameInfo))));
        printf("Envoie efectué !\n");
    } else if (!strncmp(recu, postJoinGame, POST_JOIN_GAME_SIZE)) {
        recu += POST_JOIN_GAME_SIZE;
        Game *g;
        int i = 0;
        char name[STRING_SIZE];
        printf("%s Veut rejoindre une game...\n", inet_ntoa(cl->addr.sin_addr));
        strcpy(name, cJSON_GetObjectItemCaseSensitive(cJSON_Parse(recu), "name")->valuestring);
        pthread_mutex_lock(&clientMap->gameInfo->mutex);
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
        if (playerIndex == ERR) {
            printf("Erreur joinGame\n");
            ENVOIE_ERR_INCONNUE;
            return;
        }
        ENVOI_MESSAGE(cJSON_Print(sendJoinGame(g, g->players[playerIndex])),
                      strlen(cJSON_Print(sendJoinGame(g, g->players[playerIndex]))));
        for (int i = 0; i < MAX_PLAYER; i++) {
            if (g->players[i]) {
                printf("Player %d -> Connecte\n", i);
            } else {
                printf("Player %d -> Absent\n", i);
            }
        }
        char joinGame[BUFFER_SIZE];
        sprintf(joinGame, "%s", cJSON_Print(sendJoinGame(g, g->players[playerIndex])));
        ENVOI_MESSAGE(joinGame, strlen(joinGame));
        printf("Partie Rejoint avec succès !\n");
    } else if (!strncmp(recu, postPlayerMove, POST_PLAYER_MOVE_SIZE)) {
        recu += POST_PLAYER_MOVE_SIZE;
        printf("Tentative de déplacement du joueur %s\n", inet_ntoa(cl->addr.sin_addr));

        afficheMap(*cl->clientGame->map);

        pthread_mutex_lock(&cl->clientGame->mutex);
        pthread_mutex_lock(&cl->clientGame->map->mutex);
        int caseMove = movePlayer(cl->player, cl->clientGame, cJSON_Parse(recu));
        pthread_mutex_unlock(&cl->clientGame->map->mutex);
        pthread_mutex_unlock(&cl->clientGame->mutex);

        if (caseMove == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }

        afficheMap(*cl->clientGame->map);
        char postMove[BUFFER_SIZE];
        sprintf(postMove, "%s\n%s", POST_POSITION_PLAYER_UPDATE, cJSON_Print(
                sendMove(cl->player, cJSON_GetObjectItemCaseSensitive(cJSON_Parse(recu), "move")->valuestring)));

        pthread_mutex_lock(&cl->clientGame->mutex);
        for (int i = 0; i < MAX_PLAYER; i++) {
            Player *player = cl->clientGame->players[i];
            if (player == NULL) {
                continue;
            }
            clientAddrLen = sizeof(player->addr);
            n = (int) sendto(player->socket, postMove, strlen(postMove), MSG_CONFIRM, (struct sockaddr *) &player->addr,
                             clientAddrLen);
            if (n == ERR) {
                perror("Erreur envoie du message");
                return;
            }
        }
        pthread_mutex_unlock(&cl->clientGame->mutex);
        printf("Mouvement Réussie !\n");
    } else if (!strncmp(recu, postPlayerAttack, POST_PLAYER_ATTACK_SIZE)) {
        recu += POST_PLAYER_ATTACK_SIZE;
        printf("Tentative d'attaque du joueur %s\n", inet_ntoa(cl->addr.sin_addr));
        if (attackPlayer(cl->player, cl->clientGame, cJSON_Parse(recu)) == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }
        // reponse vers le client
        //int playerIndex = 0; // TODO: Le recuperer correctement ici
        char postAttack[BUFFER_SIZE];
        sprintf(postAttack, "%sEOJ", cJSON_Print(sendPosBomb(cJSON_Parse(recu), cl->player)));
        ENVOI_MESSAGE(postAttack, strlen(postAttack));

        //TODO, Envoyer a tout les autres joueurs la position de la bombe

        pthread_mutex_lock(&cl->clientGame->mutex);
        for (int i = 0; i < MAX_PLAYER; i++) {
            Player *player = cl->clientGame->players[i];
            if (player == NULL) {
                continue;
            }
            char *postAll = malloc(sizeof(char) * BUFFER_SIZE);
            sprintf(postAll, "%s\n%sEOJ", postAttackNewbomb, cJSON_Print(cJSON_Parse(recu)));
            clientAddrLen = sizeof(player->addr);
            n = (int) sendto(player->socket, postAll, strlen(postAll), MSG_CONFIRM, (struct sockaddr *) &player->addr,
                             clientAddrLen);
            if (n == ERR) {
                perror("Erreur envoie du message");
                return;
            }
            free(postAll);
        }
        pthread_mutex_unlock(&cl->clientGame->mutex);
    } else if (!strncmp(recu, postAttackRemoteGo, POST_ATTACK_REMOTE_GO_SIZE)) {
        printf("Explosion des remotes bomb de %s\n", inet_ntoa(cl->addr.sin_addr));
        if (exploseBomb(cl->clientGame, cl->player) == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        } else {
            printf("Requête inconnue : %s\n", recu);
            ENVOIE_BAD_REQUEST;
        }
    } else if(!strncmp(recu,postObjectNew,POST_OBJECT_NEW_SIZE)){
        printf("Mise à jour du joueur %s\n", inet_ntoa(cl->addr.sin_addr));
        recu += POST_OBJECT_NEW_SIZE;
        if (updatePlayer(cl->player,cJSON_Parse(recu)) == ERR) {
            ENVOIE_ERR_INCONNUE;
            return;
        }
        char postObject[BUFFER_SIZE];
        sprintf(postObject, "%s", cJSON_Print(sendObjNew(cl->player)));
        ENVOI_MESSAGE(postObject, strlen(postObject));
    }
}

void *clientCommunication(void *args) {
    Client_Map_Games *cm = (Client_Map_Games *) args;
    Client *cl = cm->cl;
    char buffer[BUFFER_SIZE];
    int n = 0;

    for (;;) {
        strcpy(buffer, "");
        //n = recvfrom(cl->socket, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &cl->addr, &clientAddrLen);
        n = (int) read(cl->socket, buffer, BUFFER_SIZE);
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;
        } else if (n == 0) {
            printf("Connexion à %s perdu !\n", inet_ntoa(cl->addr.sin_addr));
            pthread_mutex_lock(&cl->clientGame->mutex);
            int inGame = 0;
            for (int i = 0; i < MAX_PLAYER; i++) {
                if (cl->clientGame->players[i] != NULL) {
                    if (cl->clientGame->players[i]->socket == cl->socket) {
                        pthread_mutex_unlock(&cl->clientGame->mutex);
                        inGame = 1;
                        break;
                    }
                }
            }
            if (!inGame) {
                destroyGame(cl->clientGame);
            }
            pthread_mutex_unlock(&cl->clientGame->mutex);
            break;
        }
        buffer[n] = '\0';

        printf("%s : %s\n", inet_ntoa(cl->addr.sin_addr), buffer);

        receiveSend(cm, buffer);

    }
    close(cl->socket);
    return NULL;
}