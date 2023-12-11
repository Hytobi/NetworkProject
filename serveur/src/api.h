//
// Created by hana on 04/12/23.
//

#ifndef PROJECT_API_H
#define PROJECT_API_H

#include "cJSON/cJSON.h"
#include <string.h>
#include "map.h"

#define messageClientAttendue "looking for bomberstudent servers"
#define notifClientServeurUp "hello i'm a bomberstudent server"

#define getPartieListe "GET game/list"
#define GET_PARTIE_LISTE_SIZE strlen(getPartieListe)

#define getMapListe "GET maps/list"
#define GET_MAP_LISTE_SIZE strlen(getMapListe)

#define postCreateGame "POST game/create"
#define POST_CREATE_GAME_SIZE strlen(postCreateGame)

#define postJoinGame "POST game/join"
#define POST_JOIN_GAME_SIZE strlen(postJoinGame)

#define postPlayerMove "POST player/move"
#define POST_PLAYER_MOVE_SIZE strlen(postPlayerMove)

void *clientCommunication(void *args);

cJSON *sendMapListe(maps *mapListe);

cJSON *badRequest();

cJSON *errInconnue();

#endif //PROJECT_API_H
