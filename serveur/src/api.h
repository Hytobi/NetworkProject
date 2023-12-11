//
// Created by hana on 04/12/23.
//

#ifndef PROJECT_API_H
#define PROJECT_API_H

#include "cJSON/cJSON.h"
#include "map.h"

#define messageClientAttendue "looking for bomberstudent servers"
#define notifClientServeurUp "hello i'm a bomberstudent server"
#define getPartieListe "GET game/list"
#define getMapListe "GET maps/list\n"

void *clientCommunication(void *args);

cJSON *sendMapListe(maps *mapListe);

cJSON *badRequest();

cJSON *errInconnue();

#endif //PROJECT_API_H
