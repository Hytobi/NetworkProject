/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_JSON_H
#define PROJECT_JSON_H

#include "cJSON/cJSON.h"
#include "struct.h"

cJSON *playerToJSON(Player p);

cJSON *sendMapListe(Maps *mapsInfo);

cJSON *sendGameCreation(Game *g, Map *map);

cJSON *sendPartieListe(Games *gameInfo);

cJSON *sendJoinGame(Game *g, Player *p);

cJSON *sendMove(Player *p, char move[5]);

char *sendAttackAffect(Player *p);

cJSON *sendPosBomb(cJSON *b, Player *p);

cJSON *sendObjNew(Player *p);

cJSON *newPlayer(Player p);

cJSON *sendNbBombs(int nbBomb);

cJSON *sendModifMap(Game *g, Bombe *propagation, int nbItem, int x, int y, int dist, char *type);

cJSON *badRequest();

cJSON *errInconnue();

#endif // PROJECT_JSON_H
