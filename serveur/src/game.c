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
    g->defaultPlayer = createPlayer(0, 1, 1, cl->addr);
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
        m->content[g->players[i]->x + m->width * g->players[i]->y] = '@'; // place le joueur sur la map
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
int moveOnMine(Player *p, Map *m) {
    //TODO
}

int testMove(char carac) {
    return (carac!=MUR_INCA_CHAR && carac!=MUR_CHAR && carac!=VIDE_CHAR && carac!=PLAYER_CHAR && carac!=CLASSIC_BOMB_CHAR && carac!=REMOTE_BOMB_CHAR);
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
        carac = map->content[numCase - map->width];
        if (testMove(carac)){
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase * map->width;
            }
            map->content[numCase] = SOL_CHAR;
            map->content[numCase - map->width] = PLAYER_CHAR;
            p->x--;
            return 1;
        }
        // down
    } else if (!strcmp(move, "down")) {
        carac = map->content[numCase + map->width];
        if (testMove(carac)){
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase + map->width;
            }
            map->content[numCase] = SOL_CHAR;
            map->content[numCase +map->width] = PLAYER_CHAR;
            p->x++;
            return 1;
        }
        // left
    } else if (!strcmp(move, "left")) {
        carac = map->content[numCase - 1];
        if (testMove(carac)){
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase - 1;
            }
            map->content[numCase] = SOL_CHAR;
            map->content[numCase - 1] = PLAYER_CHAR;
            p->y--;
            return 1;
        }
        // right
    } else {
        carac = map->content[numCase + 1];
        if (testMove(carac)){
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                //return numCase + 1;
            }
            map->content[numCase] = SOL_CHAR;
            map->content[numCase + 1] = PLAYER_CHAR;
            p->y++;
            return 1;
        }
    }

    return ERR;
}

void destroyGame(Game *g) {
    if (g != NULL) {
        free(g);
    }
}