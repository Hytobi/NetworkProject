
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
Player *createPlayer(int id, int x, int y, struct sockaddr_in);

/**
 * Détruit le joueur passe en paramètre
 */
void destroyPlayer(Player *);

#endif // PLAYER_H