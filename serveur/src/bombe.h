//
// Created by hana on 20/12/23.
//

#ifndef PROJECT_BOMBE_H
#define PROJECT_BOMBE_H

#include "struct.h"

void *bombeThreadExplose(void *arg);

char getRandomChar();

int processExplose(Game *g, int x, int y);

int processExploseDist(Game *g, int x, int y, int dist, char *type);

int exploseBomb(Game *g, Player *p);

/**
 * Créer une bombe normal, retourne l'id si tous se passe bien
 * ERR si une erreur
 * @param bombesInfo
 * @param x
 * @param y
 * @param dist
 * @param nbBombe
 * @return
 */
int createBombe(Bombes *bombesInfo, Player *player);

void destroyBombe(Bombe *b);

#endif //PROJECT_BOMBE_H
