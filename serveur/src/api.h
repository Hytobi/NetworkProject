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
#define GET_PARTIE_LISTE_SIZE 13

#define getMapListe "GET maps/list"
#define GET_MAP_LISTE_SIZE 13

#define postCreateGame "POST game/create"
#define POST_CREATE_GAME_SIZE 16

void *clientCommunication(void *args);

cJSON *sendMapListe(maps *mapListe);

cJSON *badRequest();

cJSON *errInconnue();

#endif //PROJECT_API_H
