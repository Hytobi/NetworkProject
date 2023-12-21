/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#include <stdio.h>
#include "player.h"
#include "map.h"
#include "err.h"
#include "json.h"

Player *createPlayer(int id, int x, int y, struct sockaddr_in addr, int socket) {
    Player *p = malloc(sizeof(Player));
    p->id = id;
    p->x = x;
    p->y = y;
    p->addr = addr;
    p->socket = socket;
    p->life = LIFE;
    p->speed = SPEED;
    p->nbClassicBomb = NB_CLASSIC_BOMB;
    p->nbRemoteBomb = NB_REMOTE_BOMB;
    p->nbMine = NB_MINE;
    p->impactDist = DEFAUT_IMPATC_DIST;
    p->invincible = INVINCIBLE;
    p->nbRemoteBombSet = 0;
    p->nbMoveInvincible = 0;

    return p;
}

void damagePlayer(Player *p, Map *map) {
    p->life -= 30;

    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    sprintf(buffer, "%s", sendAttackAffect(p));
    socklen_t clientAddrLen = sizeof(p->addr);
    int n = (int) sendto(p->socket, buffer, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &p->addr, clientAddrLen);
    free(buffer);
    if (n == ERR) {
        perror("Erreur envoie message");
    }

    if (p->life <= 0) {
        p->life = 0;
        map->content[p->y + p->x * map->width] = SOL_CHAR;
        // TODO : suppression de joueur
    }
}

void destroyPlayer(Player *p) {
    if (p != NULL) {
        free(p);
    }
}