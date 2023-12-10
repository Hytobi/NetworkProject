
#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#define MAX_PLAYER 4

#define LIFE 100
#define SPEED 75 // en pixels par seconde
#define DEFAUT_IMPATC_DIST 2;
#define NB_CLASSIC_BOMB 2
#define NB_REMOTE_BOMB 1
#define NB_MINE 1
#define INVINCIBLE 0

typedef struct player {
    int id; /**< player id */
    int x; /**< pos x du joueur */
    int y; /**< pos y du joueur */
    int life; /**< nombre de points de vie du joueur */
    int speed; /**< vitesse du joueur (1 correspond à la vitesse de base) */
    int direction; /**< TODO, jsp a quoi ca sert ? */
    int nbClassicBomb; /**< nombre de classic bombs possédées par le joueur */
    int nbRemoteBomb; /**< nombre de remote bombs possédées par le joueur */
    int nbMine; /**< nombre de mines possédées par le joueur */
    int impactDist; /**< nombre de cases impactées par une explosion (pour une direction donnée) */
    int invincible; /**<  indique si le joueur est en mode invincible */
} player;

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