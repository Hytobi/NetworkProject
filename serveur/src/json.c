//
// Created by hana on 17/12/23.
//

#include <stdio.h>
#include "json.h"

/**
 * retourne le player en format cJSON
 * @param p
 * @return
 */
cJSON *playerToJSON(Player p) {
    cJSON *playerJson = cJSON_CreateObject();

    cJSON_AddNumberToObject(playerJson, "life", p.life);
    cJSON_AddNumberToObject(playerJson, "speed", p.speed);
    cJSON_AddNumberToObject(playerJson, "nbClassicBomb", p.nbClassicBomb);
    cJSON_AddNumberToObject(playerJson, "nbMine", p.nbMine);
    cJSON_AddNumberToObject(playerJson, "nbRemoteBomb", p.nbRemoteBomb);
    cJSON_AddNumberToObject(playerJson, "impactDist", p.impactDist);
    cJSON_AddBoolToObject(playerJson, "invincible", p.invincible);

    return playerJson;
}

cJSON *sendMapListe(Maps *mapsInfo) {
    int nbMaps = mapsInfo->nbMap;
    cJSON *mapJson = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(mapJson, "action", "game/list");
    cJSON_AddStringToObject(mapJson, "statut", "200");
    cJSON_AddStringToObject(mapJson, "message", "ok");
    cJSON_AddNumberToObject(mapJson, "nbMapsList", nbMaps);

    // Création d'un tableau JSON pour "Games"
    cJSON *gamesArray = cJSON_AddArrayToObject(mapJson, "maps");

    for (int i = 0; i < nbMaps; i++) {
        Map *mapI = mapsInfo->mapListe[i];
        cJSON *game = cJSON_CreateObject();
        cJSON_AddNumberToObject(game, "id", mapI->id);
        cJSON_AddNumberToObject(game, "width", mapI->width);
        cJSON_AddNumberToObject(game, "height", mapI->height);
        cJSON_AddStringToObject(game, "content", mapI->content);
        cJSON_AddItemToArray(gamesArray, game);
    }
    return mapJson;
}

cJSON *sendGameCreation(Game *g, Map *map) {
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

    Player defautPlayer = *g->defaultPlayer;
    cJSON *playerJson = playerToJSON(defautPlayer);

    // Ajout de l'objet playerJson à l'objet principal gameCreation
    cJSON_AddItemToObject(gameCreation, "player", playerJson);

    // Retourne le JSON créé
    return gameCreation;
}

cJSON *sendPartieListe(Games *gameInfo) {
    cJSON *gameListe = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(gameListe, "action", "game/list");
    cJSON_AddStringToObject(gameListe, "statut", "200");
    cJSON_AddStringToObject(gameListe, "message", "ok");
    cJSON_AddNumberToObject(gameListe, "nbGamesList", gameInfo->nbGames);

    // Création d'un tableau JSON pour "Games"
    cJSON *gamesArray = cJSON_AddArrayToObject(gameListe, "games");

    int i = 0;
    while (i < gameInfo->nbGames) {
        Game *gameI = gameInfo->gameListe[i];
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

cJSON *sendJoinGame(Game *g, Player *p) {
    cJSON *joinGame = cJSON_CreateObject();
    cJSON_AddStringToObject(joinGame, "action", "game/join");
    cJSON_AddStringToObject(joinGame, "statut", "201");
    cJSON_AddStringToObject(joinGame, "message", "game joined");
    cJSON_AddNumberToObject(joinGame, "nbPlayers", g->nbPlayers);
    cJSON_AddNumberToObject(joinGame, "mapId", g->mapId);

    cJSON *playerListeJson = cJSON_AddArrayToObject(joinGame, "players");
    //Liste des players
    int i = 0;
    while (i < MAX_PLAYER) {
        if (g->players[i] == NULL) {
            i++;
            continue;
        }
        cJSON *playerIJSON = cJSON_CreateObject();

        char name[16];
        sprintf(name, "player%d", g->players[i]->id);
        cJSON_AddStringToObject(playerIJSON, "name", name);

        char pos[8];
        sprintf(pos, "%d,%d", g->players[i]->x, g->players[i]->y);
        cJSON_AddItemToArray(playerListeJson, playerIJSON);
        i++;
    }

    char startPos[8];
    sprintf(startPos, "%d,%d", p->x, p->y);
    cJSON_AddStringToObject(joinGame, "startPos", startPos);

    // info du joueur
    cJSON *playerInfo = playerToJSON(*p);
    cJSON_AddItemToObject(joinGame, "player", playerInfo);

    return joinGame;
}

cJSON *sendMove(Player *p, char move[5]){
    cJSON *moveJSON = cJSON_CreateObject();

    char playerX[8];
    sprintf(playerX,"player%d",p->id);

    cJSON_AddStringToObject(moveJSON,"player",playerX);
    cJSON_AddStringToObject(moveJSON,"dir",move);

    return moveJSON;
}

cJSON* sendPosBomb(cJSON *b,Player *p){
    cJSON *posBombJSON = cJSON_CreateObject();

    cJSON_AddStringToObject(posBombJSON, "action", "attack/bomb");
    cJSON_AddStringToObject(posBombJSON, "statut", "201");
    char* message = malloc(sizeof(char)*256);
    strcpy(message,cJSON_GetObjectItemCaseSensitive(b, "type")->valuestring);
    strcat(message," bomb is armed at pos ");
    strcpy(message,cJSON_GetObjectItemCaseSensitive(b, "pos")->valuestring);
    cJSON_AddStringToObject(posBombJSON, "message", message);
    

    cJSON *playerInfo = playerToJSON(*p);
    cJSON_AddItemToObject(posBombJSON, "player", playerInfo);


    return posBombJSON;
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