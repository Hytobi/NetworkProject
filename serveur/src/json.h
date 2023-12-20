//
// Created by hana on 17/12/23.
//

#ifndef PROJECT_JSON_H
#define PROJECT_JSON_H

//
// Created by hana on 17/12/23.
//

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

cJSON *badRequest();

cJSON *errInconnue();

#endif // PROJECT_JSON_H
