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

cJSON *sendMapListe(maps *mapsInfo);

cJSON *sendGameCreation(game *g, map *map);

cJSON *sendPartieListe(games *gameInfo);

cJSON *badRequest();

cJSON *errInconnue();

#endif //PROJECT_JSON_H
