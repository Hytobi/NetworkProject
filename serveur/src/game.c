//
// Created by hana on 11/12/23.
//

#include <malloc.h>
#include <string.h>
#include "game.h"
#include "stdio.h"
#include "err.h"
#include "player.h"
#include "map.h"
#include "json.h"

#define TEST_MOVES(carac) (carac!=MUR_INCA_CHAR && carac!=MUR_CHAR && carac!=VIDE_CHAR && carac!=PLAYER_CHAR && carac!=CLASSIC_BOMB_CHAR && carac!=REMOTE_BOMB_CHAR \
&& carac!=PLAYER_BOMB_CHAR && carac!=PLAYER_REMOTE_BOMB_CHAR && carac!=PLAYER_MINE_CHAR)

int createGame(Games *gameInfo, Maps *maps, cJSON *info, Client *cl) {
    int i = 0;
    while (i < gameInfo->nbGames) {
        printf("yes\n");
        if (gameInfo->gameListe[i] == NULL) {
            break;
        }
        i++;
    }
    Game *g;
    g = malloc(sizeof(Game));
    if (g == NULL) {
        perror("Erreur allocation mémoire de la partie");
        return ERR;
    }
    strcpy(g->name, cJSON_GetObjectItemCaseSensitive(info, "name")->valuestring);
    g->nbPlayers = 0;
    g->mapId = cJSON_GetObjectItemCaseSensitive(info, "mapId")->valueint;
    g->map = malloc(sizeof(Map));
    if (g->map == NULL) {
        perror("erreur malloc map in createGame");
        return ERR;
    }
    g->map = getMap(maps, g->mapId);
    g->defaultPlayer = createPlayer(0, 1, 1, cl->addr, cl->socket);
    g->startPos[0] = 1;
    g->startPos[1] = 1;

    gameInfo->gameListe[i] = g;
    return i;
}

int joinGame(Game *g, Client *cl, Map *m) {
    if (g->nbPlayers > MAX_PLAYER) {
        printf("Impossible de rejoindre, partie pleine\n");
        return ERR;
    }
    int i = 0;
    while (i <= g->nbPlayers) {
        if (g->players[i] != NULL) {
            i++;
        }
        g->nbPlayers++;
        g->players[i] = malloc(sizeof(Player));
        if (g->players[i] == NULL) {
            perror("Allocation joueur de la partie");
            return ERR;
        }
        *g->players[i] = *g->defaultPlayer;
        g->players[i]->id = g->nbPlayers;
        m->content[g->players[i]->y + m->width * g->players[i]->x] = '@'; // place le joueur sur la map
        g->defaultPlayer->x = nextPosX(i, g->mapId);
        g->defaultPlayer->y = nextPosY(i, g->mapId);
        cl->clientGame = g;
        cl->player = g->players[i];
        return i;
    }
    return ERR;
}

/**
 * Si le joueur marche sur une mine il doit prendre des dégats et la map doit s'update
 * @param p
 * @param m
 * @return
 */
void moveOnMine(Player *p, Map *m) {
    if (p->invincible) {
        return;
    }
    char buffer[BUFFER_SIZE];
    p->life-=30;
    int n = ERR;
    int i=0;
    do {
        sprintf(buffer, "%s", cJSON_Print(playerToJSON(*p)));
        socklen_t clientAddrLen = sizeof(p->addr);
        n = (int) sendto(p->socket, buffer, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &p->addr, clientAddrLen);
        i++;
    } while (n == ERR && i<50);
}

char moveAfterAttack(char maCase) {
    if (maCase == PLAYER_MINE_CHAR) {
        return MINE_CHAR;
    } else if (maCase == PLAYER_REMOTE_BOMB_CHAR) {
        return REMOTE_BOMB_CHAR;
    } else if (maCase == PLAYER_BOMB_CHAR) {
        return CLASSIC_BOMB_CHAR;
    }
    return SOL_CHAR;
}

int movePlayer(Player *p, Game *game, cJSON *info) {
    Map *map = game->map;
    char move[5];
    strcpy(move, cJSON_GetObjectItemCaseSensitive(info, "move")->valuestring);
    int actual_x = p->x, actual_y = p->y;
    int numCase = actual_y + map->width * actual_x;
    // si le joueur n'a pas la bonne place sur la map
    if (map->content[numCase] != PLAYER_CHAR) {
        //TODO
    }
    char carac;
    // up
    if (!strcmp(move, "up")) {
        int nextCase = numCase - map->width;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase * map->width;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->x--;
            return nextCase;
        }
        // down
    } else if (!strcmp(move, "down")) {
        int nextCase = numCase + map->width;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase + map->width;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->x++;
            return nextCase;
        }
        // left
    } else if (!strcmp(move, "left")) {
        int nextCase = numCase - 1;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase - 1;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->y--;
            return nextCase;
        }
        // right
    } else {
        int nextCase = numCase + 1;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase + 1;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->y++;
            return nextCase;
        }
    }

    return ERR;
}

int attackPlayer(Player *p, Game *g, cJSON *info) {
    char move[5];
    strcpy(move, cJSON_GetObjectItemCaseSensitive(info, "pos")->valuestring);
    int x = atoi(strtok(move, ","));
    int y = atoi(strtok(NULL, ","));
    Map *map = g->map;
    int numCase = y + map->width * x;
    char carac = map->content[numCase];
    if (carac == PLAYER_CHAR) {
        // peut etre changer content en une liste de structure 'case' coup set des champ tel que "a bombe"
        char *type = cJSON_GetObjectItemCaseSensitive(info, "type")->valuestring;
        if (!strcmp(type, "classic")) {
            p->nbClassicBomb--;
            map->content[numCase] = PLAYER_BOMB_CHAR;
        } else if (!strcmp(type, "remote")) {
            p->nbRemoteBomb--;
            map->content[numCase] = PLAYER_REMOTE_BOMB_CHAR;
        } else if (!strcmp(type, "mine")) {
            p->nbMine--;
            map->content[numCase] = PLAYER_MINE_CHAR;
        }
        return 1;

    }
    return ERR;
}

void destroyGame(Game *g) {
    if (g != NULL) {
        free(g);
    }
}