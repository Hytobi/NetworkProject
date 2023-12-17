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

cJSON *sendMapListe(Maps *mapsInfo);

cJSON *sendGameCreation(Game *g, Map *map);

cJSON *sendPartieListe(Games *gameInfo);

cJSON *sendJoinGame(Game *g, Player *p);

cJSON *badRequest();

cJSON *errInconnue();

#endif //PROJECT_JSON_H
