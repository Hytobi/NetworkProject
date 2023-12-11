
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
player *createPlayer(int id, int x, int y);

/**
 * Détruit le joueur passe en paramètre
 */
void destroyPlayer(player *);

#endif // PLAYER_H