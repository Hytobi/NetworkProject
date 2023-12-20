//
// Created by hana on 20/12/23.
//

#ifndef PROJECT_BOMBE_H
#define PROJECT_BOMBE_H

#include "struct.h"

void* bombeThread(void* arg);

int createBombe(Bombes * bombesInfo);

void destroyBombe(Bombe * b);

#endif //PROJECT_BOMBE_H
