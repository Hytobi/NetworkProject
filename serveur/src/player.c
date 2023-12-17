#include "player.h"

player *createPlayer(int id, int x, int y, struct sockaddr_in addr) {
    player *p = malloc(sizeof(player));
    p->id = id;
    p->x = x;
    p->y = y;
    p->addr = addr;
    p->life = LIFE;
    p->speed = SPEED;
    p->direction = 0; //TODO jsp a quoi ce sert ?
    p->nbClassicBomb = NB_CLASSIC_BOMB;
    p->nbRemoteBomb = NB_REMOTE_BOMB;
    p->nbMine = NB_MINE;
    p->impactDist = DEFAUT_IMPATC_DIST;
    p->invincible = INVINCIBLE;

    return p;
}

void destroyPlayer(player *p) {
    free(p);
}