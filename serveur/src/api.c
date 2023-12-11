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

#define ENVOI_MESSAGE(x) sprintf(buffer2, "%s", x); \
int n = sendto(cl->socket, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &cl->addr, clientAddrLen); \
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

cJSON *sendGameCreation(game *g) {
    cJSON *gameCreation = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(gameCreation, "action", "game/create");
    cJSON_AddStringToObject(gameCreation, "statut", "201");
    cJSON_AddStringToObject(gameCreation, "message", "game created");
    cJSON_AddNumberToObject(gameCreation, "nbPlayers", g->nbPlayers);

    char startPos[5];
    sprintf(startPos, "%d,%d", g->startPos[0], g->startPos[1]);
    cJSON_AddStringToObject(gameCreation, "startPos", startPos);

    player *defautPlayer = g->defaultPlayer;
    cJSON *playerJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(playerJson, "life", defautPlayer->life);
    cJSON_AddNumberToObject(playerJson, "speed", defautPlayer->speed);
    cJSON_AddNumberToObject(playerJson, "nbClassicBomb", defautPlayer->nbClassicBomb);
    cJSON_AddNumberToObject(playerJson, "nbMine", defautPlayer->nbMine);
    cJSON_AddNumberToObject(playerJson, "nbRemoteBomb", defautPlayer->nbRemoteBomb);
    cJSON_AddNumberToObject(playerJson, "impactDist", defautPlayer->impactDist);
    cJSON_AddBoolToObject(playerJson, "invincible", defautPlayer->invincible);

    // Ajout de l'objet playerJson à l'objet principal gameCreation
    cJSON_AddItemToObject(gameCreation, "player", playerJson);

    // Retourne le JSON créé
    return gameCreation;
}

cJSON *sendPartieListe(games *gameInfo) {
    cJSON *gameListe = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(gameListe, "action", "game/list");
    cJSON_AddStringToObject(gameListe, "statut", "200");
    cJSON_AddStringToObject(gameListe, "message", "ok");
    cJSON_AddNumberToObject(gameListe, "nbGamesList", gameInfo->nbGames);

    // Création d'un tableau JSON pour "games"
    cJSON *gamesArray = cJSON_AddArrayToObject(gameListe, "games");

    int i = 0;
    while (i < gameInfo->nbGames) {
        game *gameI = gameInfo->gameListe[i];
        if (gameI == NULL) {
            continue;
        }
        cJSON *game = cJSON_CreateObject();
        cJSON_AddStringToObject(game, "name", gameI->name);
        cJSON_AddNumberToObject(game, "nbPlayer", gameI->nbPlayers);
        cJSON_AddNumberToObject(game, "mapId", gameI->mapId);
        cJSON_AddItemToArray(gamesArray, game);
        i++;
    }
    printf("%s\n", cJSON_Print(gameListe));
    return gameListe;
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
    } else if (!strncmp(recu, getMapListe, GET_MAP_LISTE_SIZE)) {
        printf("Envoie des informations concernant les maps à : %s ...\n", inet_ntoa(cl->addr.sin_addr));
        ENVOI_MESSAGE(cJSON_Print(sendMapListe(clientMap->mapInfo)));
        printf("Map envoyé avec Succès\n");
    } else if (!strncmp(recu, postCreateGame, POST_CREATE_GAME_SIZE)) {
        recu += 16;
        printf("Demande de création de game: %s\n", recu);
        printf("Création de la partie...\n");
        int indiceGame = createGame(clientMap->gameInfo, cJSON_Parse(recu));
        if (indiceGame == ERR) {
            printf("erreur lors de la création de la game\n");
            ENVOI_MESSAGE(cJSON_Print(errInconnue()));
            return;
        }
        ENVOI_MESSAGE(cJSON_Print(sendGameCreation(clientMap->gameInfo->gameListe[indiceGame])));
        printf("Partie créer !\n");
    } else if (!strncmp(recu, getPartieListe, GET_PARTIE_LISTE_SIZE)) {
        printf("Envoie de la liste des parties...\n");
        ENVOI_MESSAGE(cJSON_Print(sendPartieListe(clientMap->gameInfo)));
        printf("Envoie efectué !\n");
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