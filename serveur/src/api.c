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

#define ENVOI_MESSAGE(x) sprintf(buffer2, "%s", x); \
int n = sendto(cl->socket, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &cl->addr, clientAddrLen); \
//n = send(cl->socket,buffer2,BUFFER_SIZE,0); \
if (n == ERR) { \
perror("Erreur envoie du message"); \
return; \
}

cJSON *sendMapListe(maps *mapsInfo) {
    int nbMaps = mapsInfo->nbMap;
    cJSON *mapJson = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(mapJson, "action", "game/list");
    cJSON_AddStringToObject(mapJson, "statut", "200");
    cJSON_AddStringToObject(mapJson, "message", "ok");
    cJSON_AddNumberToObject(mapJson, "nbMapsList", nbMaps);

    // Création d'un tableau JSON pour "games"
    cJSON *gamesArray = cJSON_AddArrayToObject(mapJson, "maps");

    for (int i = 0; i < nbMaps; i++) {
        map *mapI = mapsInfo->mapListe[i];
        cJSON *game = cJSON_CreateObject();
        cJSON_AddNumberToObject(game, "id", mapI->id);
        cJSON_AddNumberToObject(game, "width", mapI->width);
        cJSON_AddNumberToObject(game, "height", mapI->height);
        cJSON_AddStringToObject(game, "content", mapI->content);
        cJSON_AddItemToArray(gamesArray, game);
    }
    return mapJson;
}

cJSON *badRequest() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "statut", 400);
    cJSON_AddStringToObject(json, "message", "Bad Request");
    return json;
}

cJSON *errInconnue() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "statut", 520);
    cJSON_AddStringToObject(json, "message", "Unknown Error");
    return json;
}

/**
 * Analyse la chaine reçu et renvoie la réponse approprié
 * @param recu
 */
void receiveSend(client_map_games *clientMap, char *recu) {
    client *cl = clientMap->cl;
    socklen_t clientAddrLen = sizeof(cl->addr);
    char buffer2[BUFFER_SIZE];
    if (!strcmp(recu, messageClientAttendue)) {
        ENVOI_MESSAGE(notifClientServeurUp);
    }
    else if (!strcmp(recu, getMapListe)) {
        printf("Envoie des informations concernant les maps à : %s ...\n", inet_ntoa(cl->addr.sin_addr));
        ENVOI_MESSAGE(cJSON_Print(sendMapListe(clientMap->mapInfo)));
        printf("Map envoyé avec Succès\n");
    }
}

void *clientCommunication(void *args) {
    client_map_games *cm = (client_map_games *) args;
    client *cl = cm->cl;
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