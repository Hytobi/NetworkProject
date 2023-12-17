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


int createGame(games *gameInfo, cJSON *info, struct sockaddr_in addr) {
    int i = 0;
    while (i < gameInfo->nbGames) {
        printf("yes\n");
        if (gameInfo->gameListe[i] == NULL) {
            break;
        }
        i++;
    }
    game *g;
    g = malloc(sizeof(game));
    if (g == NULL) {
        perror("Erreur allocation mémoire de la partie");
        return ERR;
    }
    strcpy(g->name, cJSON_GetObjectItemCaseSensitive(info, "name")->valuestring);
    g->nbPlayers = 0;
    g->mapId = cJSON_GetObjectItemCaseSensitive(info, "mapId")->valueint;
    g->defaultPlayer = createPlayer(0, 1, 1,addr);
    g->startPos[0] = 1;
    g->startPos[1] = 1;

    gameInfo->gameListe[i] = g;
    return i;
}

int joinGame(game *g, client *cl, map *m) {
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
        g->players[i] = malloc(sizeof(player));
        if (g->players[i] == NULL) {
            perror("Allocation joueur de la partie");
            return ERR;
        }
        *g->players[i] = *g->defaultPlayer;
        g->players[i]->id=g->nbPlayers;
        m->content[m->height*g->players[i]->x+g->players[i]->y]='@'; // place le joueur sur la map
        g->defaultPlayer->x = nextPosX(i, g->mapId);
        g->defaultPlayer->y = nextPosY(i, g->mapId);
        cl->clientGame = g;
        return 1;
    }

}

int movePlayer(map *map, cJSON *info){
    char *move = malloc(sizeof(char) * 7); // "(XX,XX)"
    if (move == NULL) {
        perror("Erreur allocation mémoire du mouvement");
        return ERR;
    }
    strcpy(move,cJSON_GetObjectItemCaseSensitive(info,"move")->valuestring);
    int x = atoi(strtok(move, ","));
    int y = atoi(strtok(NULL, ","));
    if (x < 0 || x > map->width || y < 0 || y > map->height) {
        // Ne peut pas se déplacer
        return 0;
    }
    char carac = map->content[x+y*map->width];
    if (!strcmp(carac,MUR_INCA) || !strcmp(carac,MUR) || !strcmp(carac,VIDE)) { // # / ou X
        // Ne peut pas se déplacer
        return 0;
    }
    return 1;
}

void destroyGame(game *g) {
    if (g != NULL) {
        free(g);
    }
}