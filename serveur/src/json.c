//
// Created by hana on 17/12/23.
//

#include <stdio.h>
#include "json.h"

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

cJSON *sendGameCreation(game *g, map *map) {
    cJSON *gameCreation = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(gameCreation, "action", "game/create");
    cJSON_AddStringToObject(gameCreation, "statut", "201");
    cJSON_AddStringToObject(gameCreation, "message", "game created");
    cJSON_AddNumberToObject(gameCreation, "nbPlayers", g->nbPlayers);

    cJSON *game = cJSON_CreateObject();
    cJSON_AddNumberToObject(game, "id", map->id);
    cJSON_AddNumberToObject(game, "width", map->width);
    cJSON_AddNumberToObject(game, "height", map->height);
    cJSON_AddStringToObject(game, "content", map->content);
    cJSON_AddItemToObject(gameCreation, "startingMap", game);

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