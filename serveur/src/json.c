/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#include <stdio.h>
#include <malloc.h>
#include "json.h"
#include "api.h"

/**
 * retourne le player en format cJSON
 * @param p
 * @return
 */
cJSON *playerToJSON(Player p)
{
    cJSON *playerJson = cJSON_CreateObject();

    cJSON_AddNumberToObject(playerJson, "life", p.life);
    cJSON_AddNumberToObject(playerJson, "speed", p.speed);
    cJSON_AddNumberToObject(playerJson, "nbClassicBomb", p.nbClassicBomb);
    cJSON_AddNumberToObject(playerJson, "nbMine", p.nbMine);
    cJSON_AddNumberToObject(playerJson, "nbRemoteBomb", p.nbRemoteBomb);
    cJSON_AddNumberToObject(playerJson, "impactDist", p.impactDist);
    cJSON_AddBoolToObject(playerJson, "invincible", p.invincible);
    cJSON_AddNumberToObject(playerJson, "nbMoveInvincible", p.nbMoveInvincible);

    return playerJson;
}

cJSON *mapToJSON(Map map)
{
    cJSON *mapJSON = cJSON_CreateObject();
    cJSON_AddNumberToObject(mapJSON, "id", map.id);
    cJSON_AddNumberToObject(mapJSON, "width", map.width);
    cJSON_AddNumberToObject(mapJSON, "height", map.height);
    cJSON_AddStringToObject(mapJSON, "content", map.content);
    return mapJSON;
}

cJSON *sendMapListe(Maps *mapsInfo)
{
    int nbMaps = mapsInfo->nbMap;
    cJSON *mapJson = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(mapJson, "action", "game/list");
    cJSON_AddStringToObject(mapJson, "statut", "200");
    cJSON_AddStringToObject(mapJson, "message", "ok");
    cJSON_AddNumberToObject(mapJson, "nbMapsList", nbMaps);

    // Création d'un tableau JSON pour "Games"
    cJSON *gamesArray = cJSON_AddArrayToObject(mapJson, "maps");

    pthread_mutex_lock(&mapsInfo->mutex);
    for (int i = 0; i < nbMaps; i++)
    {
        Map *mapI = mapsInfo->mapListe[i];
        cJSON_AddItemToArray(gamesArray, mapToJSON(*mapI));
    }
    pthread_mutex_unlock(&mapsInfo->mutex);
    return mapJson;
}

cJSON *sendGameCreation(Game *g, Map *map)
{
    cJSON *gameCreation = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(gameCreation, "action", "game/create");
    cJSON_AddStringToObject(gameCreation, "statut", "201");
    cJSON_AddStringToObject(gameCreation, "message", "game created");
    cJSON_AddNumberToObject(gameCreation, "nbPlayers", g->nbPlayers);

    cJSON_AddItemToObject(gameCreation, "startingMap", mapToJSON(*map));

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

cJSON *sendPartieListe(Games *gameInfo)
{
    cJSON *gameListe = cJSON_CreateObject();

    // Ajout des éléments au JSON
    cJSON_AddStringToObject(gameListe, "action", "game/list");
    cJSON_AddStringToObject(gameListe, "statut", "200");
    cJSON_AddStringToObject(gameListe, "message", "ok");
    cJSON_AddNumberToObject(gameListe, "nbGamesList", gameInfo->nbGames);

    // Création d'un tableau JSON pour "Games"
    cJSON *gamesArray = cJSON_AddArrayToObject(gameListe, "games");

    int i = 0;
    while (i < MAX_GAMES)
    {
        Game *gameI = gameInfo->gameListe[i];
        if (gameI == NULL)
        {
            i++;
            continue;
        }
        cJSON *game = cJSON_CreateObject();
        cJSON_AddStringToObject(game, "name", gameI->name);
        char mapId[3];
        sprintf(mapId, "%d", gameI->mapId);
        cJSON_AddStringToObject(game, "mapId", mapId);
        cJSON_AddNumberToObject(game, "nbPlayer", gameI->nbPlayers);
        cJSON_AddItemToArray(gamesArray, game);
        i++;
    }
    return gameListe;
}

cJSON *sendJoinGame(Game *g, Player *p)
{
    cJSON *joinGame = cJSON_CreateObject();
    cJSON_AddStringToObject(joinGame, "action", "game/join");
    cJSON_AddStringToObject(joinGame, "statut", "201");
    cJSON_AddStringToObject(joinGame, "message", "game joined");
    cJSON_AddNumberToObject(joinGame, "nbPlayers", g->nbPlayers - 1);
    cJSON_AddNumberToObject(joinGame, "mapId", g->mapId);

    cJSON *playerListeJson = cJSON_AddArrayToObject(joinGame, "players");
    // Liste des players
    int i = 0;
    while (i < MAX_PLAYER)
    {
        if (g->players[i] == NULL || g->players[i] == p)
        {
            i++;
            continue;
        }
        cJSON *playerIJSON = cJSON_CreateObject();

        char name[16];
        sprintf(name, "player%d", g->players[i]->id);
        cJSON_AddStringToObject(playerIJSON, "name", name);

        char pos[8];
        sprintf(pos, "%d,%d", g->players[i]->x, g->players[i]->y);
        cJSON_AddStringToObject(playerIJSON, "pos", pos);
        cJSON_AddItemToArray(playerListeJson, playerIJSON);
        i++;
    }

    char startPos[8];
    sprintf(startPos, "%d,%d", p->x, p->y);
    cJSON_AddStringToObject(joinGame, "startPos", startPos);

    // info du joueur
    cJSON *playerInfo = playerToJSON(*p);
    cJSON_AddItemToObject(joinGame, "player", playerInfo);

    pthread_mutex_lock(&g->map->mutex);
    cJSON_AddItemToObject(joinGame, "startingMap", mapToJSON(*g->map));
    pthread_mutex_unlock(&g->map->mutex);

    return joinGame;
}

cJSON *newPlayer(Player p)
{
    cJSON *pJSON = cJSON_CreateObject();

    char name[STRING_SIZE];
    sprintf(name, "player%d", p.id);
    cJSON_AddStringToObject(pJSON, "player", name);

    char pos[8];
    sprintf(pos, "%d,%d", p.x, p.y);
    cJSON_AddStringToObject(pJSON, "pos", pos);

    return pJSON;
};

cJSON *sendMove(Player *p, char move[5])
{
    cJSON *moveJSON = cJSON_CreateObject();

    char playerX[8];
    sprintf(playerX, "player%d", p->id);

    cJSON_AddStringToObject(moveJSON, "player", playerX);
    cJSON_AddStringToObject(moveJSON, "dir", move);

    return moveJSON;
}

cJSON *sendPosBomb(cJSON *b, Player *p)
{
    cJSON *posBombJSON = cJSON_CreateObject();

    cJSON_AddStringToObject(posBombJSON, "action", "attack/bomb");
    cJSON_AddStringToObject(posBombJSON, "statut", "201");
    char *message = malloc(sizeof(char) * 256);
    strcpy(message, cJSON_GetObjectItemCaseSensitive(b, "type")->valuestring);
    strcat(message, " bomb is armed at pos ");
    strcat(message, cJSON_GetObjectItemCaseSensitive(b, "pos")->valuestring);
    cJSON_AddStringToObject(posBombJSON, "message", message);

    // info du joueur
    cJSON *playerInfo = playerToJSON(*p);
    cJSON_AddItemToObject(posBombJSON, "player", playerInfo);

    return posBombJSON;
}

char *sendAttackAffect(Player *p)
{
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    sprintf(buffer, "%s\n%s", postAttackAffect, cJSON_Print(playerToJSON(*p)));
    return buffer;
}

cJSON *sendObjNew(Player *p)
{
    cJSON *objNewJSON = cJSON_CreateObject();

    cJSON_AddStringToObject(objNewJSON, "action", "object/new");
    cJSON_AddStringToObject(objNewJSON, "statut", "201");
    cJSON_AddStringToObject(objNewJSON, "message", "ok");

    // info du joueur
    cJSON *playerInfo = playerToJSON(*p);
    cJSON_AddItemToObject(objNewJSON, "player", playerInfo);

    return objNewJSON;
}

cJSON *sendModifMap(Game *g, Bombe *propagation, int nbItem, int x, int y, int dist, char *type)
{
    cJSON *json = cJSON_CreateObject();
    char pos[8];
    sprintf(pos, "%d,%d", x, y);
    cJSON_AddStringToObject(json, "pos", pos);
    cJSON_AddStringToObject(json, "type", type);
    cJSON_AddNumberToObject(json, "impactDist", dist);

    cJSON *casesModifies = cJSON_AddArrayToObject(json, "casesModifies");
    // Liste des players
    int i = 0;
    Map map = *g->map;
    while (i < nbItem)
    {
        cJSON *caseJSON = cJSON_CreateObject();
        cJSON_AddNumberToObject(caseJSON, "x", propagation[i].x);
        cJSON_AddNumberToObject(caseJSON, "y", propagation[i].y);
        char tab[1];
        sprintf(tab, "%c", map.content[propagation[i].y + map.width * propagation[i].x]);
        cJSON_AddStringToObject(caseJSON, "carac", tab);
        cJSON_AddItemToArray(casesModifies, caseJSON);
        i++;
    }

    return json;
}

cJSON *sendNbBombs(int nbBomb)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "nbBombe", nbBomb);
    return json;
}

cJSON *badRequest()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "statut", 400);
    cJSON_AddStringToObject(json, "message", "Bad Request");
    return json;
}

cJSON *errInconnue()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "statut", 520);
    cJSON_AddStringToObject(json, "message", "Unknown Error");
    return json;
}