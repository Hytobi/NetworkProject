//
// Created by hana on 20/12/23.
//

#ifndef PROJECT_BOMBE_H
#define PROJECT_BOMBE_H

#include "struct.h"

void* bombeThread(void* arg);

char getRandomChar();

int processExplose(Game *g, int x, int y);

int processExploseDist(Game *g, int x, int y, int dist);

int exploseBomb(Game *g, Player *p);

int createBombe(Bombes * bombesInfo);

void destroyBombe(Bombe * b);

#endif //PROJECT_BOMBE_H
