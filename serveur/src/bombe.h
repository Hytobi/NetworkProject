/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_BOMBE_H
#define PROJECT_BOMBE_H

#include "struct.h"

/**
 * Fonction qui gère le thread de la bombe classic
*/
void *bombeThreadExplose(void *arg);

/**
 * Fonction qui gère l'explosion d'une bombes en fonction de sa position
*/
int processExplose(Game *g, int x, int y);

/**
 * Fonction qui gère l'explosion d'une bombes et colision avec les joueurs/murs
*/
int processExploseDist(Game *g, int x, int y, int dist, char *type);

/**
 * Fonction qui gère l'explosion d'une remote bombe
*/
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

/**
 * Fonction qui detruit une bombe
*/
void destroyBombe(Bombe *b);

#endif // PROJECT_BOMBE_H
