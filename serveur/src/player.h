/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "struct.h"

/**
 * Créer un nouveau joueur avec des valeurs par défault
 * @param id
 * @param x
 * @param y
 * @return
 */
Player *createPlayer(int id, int x, int y, struct sockaddr_in, int socket);

/**
 * Inflige 30 points de dégats au joueur, envoie le message au player
 * et met a jour la map s'il meurt
 * @param p
 * @param map
 */
void damagePlayer(Player *p, Map *map);

/**
 * Détruit le joueur passe en paramètre
 */
void destroyPlayer(Player *);

#endif // PLAYER_H