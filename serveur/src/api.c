//
// Created by hana on 04/12/23.
//

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "api.h"
#include "struct.h"
#include "err.h"
#include "game.h"
#include "json.h"

#define ENVOI_MESSAGE(x) sprintf(buffer2, "%s", x); \
int n = sendto(cl->socket, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &cl->addr, clientAddrLen); \
if (n == ERR) { \
perror("Erreur envoie du message"); \
return; \
}

#define ENVOIE_ERR_INCONNUE ENVOI_MESSAGE(cJSON_Print(errInconnue()))
#define ENVOIE_BAD_REQUEST ENVOI_MESSAGE(cJSON_Print(badRequest()))

#define GET_MAP(m, i) m = getMap(clientMap->mapInfo,clientMap->gameInfo->gameListe[i]->mapId);\
if (m==NULL){ \
 \
}

/**
 * Analyse la chaine reçu et renvoie la réponse approprié
 * @param recu
 */
void receiveSend(Client_Map_Games *clientMap, char *recu) {
    Client *cl = clientMap->cl; // structure contenant toutes les infos necessaire
    socklen_t clientAddrLen = sizeof(cl->addr); // adresse du Client
    char buffer2[BUFFER_SIZE];
    if (!strcmp(recu, messageClientAttendue)) {
        ENVOI_MESSAGE(notifClientServeurUp);
    } else if (!strncmp(recu, getMapListe, GET_MAP_LISTE_SIZE)) {
        printf("Envoie des informations concernant les Maps à : %s ...\n", inet_ntoa(cl->addr.sin_addr));
        ENVOI_MESSAGE(cJSON_Print(sendMapListe(clientMap->mapInfo)));
        printf("Map envoyé avec Succès\n");
    } else if (!strncmp(recu, postCreateGame, POST_CREATE_GAME_SIZE)) {
        recu += POST_CREATE_GAME_SIZE;
        printf("Demande de création de game: %s\n", recu);
        printf("Création de la partie...\n");
        int indiceGame = createGame(clientMap->gameInfo, clientMap->mapInfo,cJSON_Parse(recu), cl);
        if (indiceGame == ERR) {
            printf("erreur lors de la création de la game\n");
            ENVOI_MESSAGE(cJSON_Print(errInconnue()));
            return;
        }
        Map *m;
        GET_MAP(m, indiceGame);
        if (joinGame(clientMap->gameInfo->gameListe[indiceGame], cl, m)==ERR){
            printf("Erreur join game\n");
            ENVOIE_ERR_INCONNUE;
            return;
        }
        ENVOI_MESSAGE(cJSON_Print(sendGameCreation(cl->clientGame,
                                                   clientMap->mapInfo->mapListe[cl->clientGame->mapId])));
        printf("Partie créer !\n");
    } else if (!strncmp(recu, getPartieListe, GET_PARTIE_LISTE_SIZE)) {
        printf("Envoie de la liste des parties...\n");
        ENVOI_MESSAGE(cJSON_Print(sendPartieListe(clientMap->gameInfo)));
        printf("Envoie efectué !\n");
    } else if (!strncmp(recu, postJoinGame, POST_JOIN_GAME_SIZE)) {
        recu += POST_JOIN_GAME_SIZE;
        Game *g;
        int i = 0;
        char *name;
        printf("%s Veut rejoindre une game...\n", inet_ntoa(cl->addr.sin_addr));
        strcpy(name, cJSON_GetObjectItemCaseSensitive(cJSON_Parse(recu), "name")->valuestring);
        while (i < MAX_GAMES) {
            if (clientMap->gameInfo->gameListe[i] == NULL) {
                i++;
                continue;
            }
            if (!strcmp(clientMap->gameInfo->gameListe[i]->name, name)) {
                g = clientMap->gameInfo->gameListe[i];
            }
            i++;
        }
        Map *m;
        GET_MAP(m, g->mapId);
        int playerIndex = joinGame(g, cl, m);
        if (playerIndex==ERR){
            printf("Erreur joinGame\n");
            ENVOIE_ERR_INCONNUE;
            return;
        }
        ENVOI_MESSAGE(cJSON_Print(sendJoinGame(g,g->players[playerIndex])));
        printf("Partie Rejoint avec succès !\n");
    } else if (!strncmp(recu,postPlayerMove,POST_PLAYER_MOVE_SIZE)){
        recu+=POST_PLAYER_MOVE_SIZE;
        printf("Tentative de déplacement du joueur %s\n", inet_ntoa(cl->addr.sin_addr));

        if (movePlayer(cl->player,cl->clientGame,recu)==ERR){
            ENVOIE_ERR_INCONNUE;
            return;
        }
        printf("1\n");
        char postMove[BUFFER_SIZE];
        printf("2\n");
        sprintf(postMove,"%s\n%s",POST_POSITION_PLAYER_UPDATE, cJSON_Print(sendMove(cl->player,recu)));
        printf("3\n");
        ENVOI_MESSAGE(postMove);
        printf("Mouvement Réussie !\n");
    }
    else {
        printf("Requête inconnue : %s\n", recu);
        ENVOIE_BAD_REQUEST;
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
        n = read(cl->socket, buffer, BUFFER_SIZE);
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;
        } else if (n == 0) {
            //TODO si c'est un joueur qui host une partie il faut déplacer la partie à un autre joueur
            printf("Connexion à %s perdu !\n", inet_ntoa(cl->addr.sin_addr));
            break;
        }
        buffer[n] = '\0';

        printf("%s : %s\n", inet_ntoa(cl->addr.sin_addr), buffer);

        receiveSend(cm, buffer);

    }
    close(cl->socket);
    return NULL;
}