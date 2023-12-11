//
// Created by hana on 11/12/23.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include "struct.h"
#include "cJSON/cJSON.h"

void initGames(games *gameInfo);

int createGame(games *gameInfo, cJSON *info);

void destroyGame(game *g);

#endif //PROJECT_GAME_H
