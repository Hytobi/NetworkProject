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

cJSON sendMapListe() {
    cJSON *mapListe = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(mapListe, "action", "game/list");
    cJSON_AddStringToObject(mapListe, "statut", "200");
    cJSON_AddStringToObject(mapListe, "message", "ok");
    cJSON_AddNumberToObject(mapListe, "nbGamesList", 2);

    // Création d'un tableau JSON pour "games"
    cJSON *gamesArray = cJSON_AddArrayToObject(mapListe, "games");

    // Ajout du premier objet dans le tableau "games"
    cJSON *game1 = cJSON_CreateObject();
    cJSON_AddStringToObject(game1, "name", "game1");
    cJSON_AddNumberToObject(game1, "nbPlayer", 2);
    cJSON_AddNumberToObject(game1, "mapId", 0);
    cJSON_AddItemToArray(gamesArray, game1);

    // Ajout du deuxième objet dans le tableau "games"
    cJSON *game2 = cJSON_CreateObject();
    cJSON_AddStringToObject(game2, "name", "game2");
    cJSON_AddNumberToObject(game2, "nbPlayer", 2);
    cJSON_AddNumberToObject(game2, "mapId", 1);
    cJSON_AddItemToArray(gamesArray, game2);

    // Convertir le JSON en chaîne de caractères
    char *jsonString = cJSON_Print(mapListe);
}

cJSON *badRequest(){
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"statut",400);
    cJSON_AddStringToObject(json,"message","Bad Request");
    return json;
}

cJSON *errInconnue(){
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"statut",520);
    cJSON_AddStringToObject(json,"message","Unknown Error");
    return json;
}

void *clientCommunication(void *args) {
    client *cl = (client *) args;
    char buffer[BUFFER_SIZE];
    int n = 0;

    for (;;) {
        socklen_t clientAddrLen = sizeof(cl->addr);
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

        if (!strcmp(buffer, getPartieListe)) {

        }

        printf("%s : %s", inet_ntoa(cl->addr.sin_addr), buffer);

        char buffer2[1024];
        sprintf(buffer2, "%s", notifClientServeurUp);
        //sprintf(buffer2,"yes");
        n = sendto(cl->socket, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &cl->addr, clientAddrLen);
        //n = send(cl->socket,buffer2,BUFFER_SIZE,0);
        if (n == ERR) {
            perror("Erreur envoie du message");
            continue;
        }
    }
    close(cl->socket);
}