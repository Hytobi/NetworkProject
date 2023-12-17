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

#define TEST_MOVE(carac) if (carac==MUR_INCA_CHAR || carac==MUR_CHAR\
|| carac==VIDE_CHAR || carac==PLAYER_CHAR || carac==CLASSIC_BOMB_CHAR\
|| carac==REMOTE_BOMB_CHAR) {\
printf("Déplacement impossible !\n");\
return ERR;\
}

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
        m->content[m->height * g->players[i]->x + g->players[i]->y] = '@'; // place le joueur sur la map
        g->defaultPlayer->x = nextPosX(i, g->mapId);
        g->defaultPlayer->y = nextPosY(i, g->mapId);
        cl->clientGame = g;
        cl->player=g->players[i];
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

int movePlayer(Player *p, Game *game, cJSON *info) {
    printf("11\n");
    Map *map = game->map;
    char move[5];
    strcpy(move, cJSON_GetObjectItemCaseSensitive(info, "move")->valuestring);
    printf("12\n");
    int actual_x = p->x, actual_y = p->y;
    int numCase = map->height * actual_x + actual_y;
    printf("13\n");
    // si le joueur n'a pas la bonne place sur la map
    if (map->content[numCase] != PLAYER_CHAR) {
        //TODO
        printf("131\n");
    }
    char carac;
    // up
    printf("14\n");
    if (!strcmp(move, "up")) {
        printf("141\n");
        carac = map->content[numCase + 1];
        TEST_MOVE(carac);
        if (carac == MINE_CHAR) {
            moveOnMine(p, map);
            return numCase + 1;
        }
        map->content[numCase] = SOL_CHAR;
        map->content[numCase + 1] = PLAYER_CHAR;
        // down
    } else if (!strcmp(move, "down")) {
        printf("142\n");
        carac = map->content[numCase - 1];
        TEST_MOVE(carac);
        if (carac == MINE_CHAR) {
            moveOnMine(p, map);
            return numCase - 1;
        }
        map->content[numCase] = SOL_CHAR;
        map->content[numCase - 1] = PLAYER_CHAR;
        // left
    } else if (!strcmp(move, "left")) {
        printf("143\n");
        carac = map->content[numCase - map->height];
        TEST_MOVE(carac);
        if (carac == MINE_CHAR) {
            moveOnMine(p, map);
            return numCase - map->height;
        }
        map->content[numCase] = SOL_CHAR;
        map->content[numCase - map->height] = PLAYER_CHAR;
        // right
    } else {
        printf("144\n");
        carac = map->content[numCase + map->height];
        TEST_MOVE(carac);
        if (carac == MINE_CHAR) {
            moveOnMine(p, map);
            return numCase + map->height;
        }
        map->content[numCase] = SOL_CHAR;
        map->content[numCase + map->height] = PLAYER_CHAR;
    }

    return ERR;
}

void destroyGame(Game *g) {
    if (g != NULL) {
        free(g);
    }
}