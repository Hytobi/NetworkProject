//
// Created by hana on 11/12/23.
//

#include <malloc.h>
#include <string.h>
#include "game.h"
#include "stdio.h"
#include "err.h"
#include "player.h"


int createGame(games *gameInfo, cJSON *info) {
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
    if (g==NULL){
        perror("Erreur allocation mémoire de la partie");
        return ERR;
    }
    strcpy(g->name,cJSON_GetObjectItemCaseSensitive(info,"name")->valuestring);
    g->nbPlayers = 0;
    g->mapId = cJSON_GetObjectItemCaseSensitive(info,"mapId")->valueint;
    g->defaultPlayer=createPlayer(0,1,1);
    g->startPos[0]=1;
    g->startPos[1]=1;

    gameInfo->gameListe[i]=g;
    return i;
}

int joinGame(game *g, client *cl){
    if (g->nbPlayers >= MAX_PLAYER){
        printf("Impossible de rejoindre, partie pleine\n");
        return ERR;
    }
    int i=0;
    while (i<=g->nbPlayers){
        if (g->players[i]!=NULL){
            i++;
        }
        g->nbPlayers++;
        g->players[i]= malloc(sizeof(player));
        if (g->players[i]==NULL){
            perror("Allocation joueur de la partie");
            return ERR;
        }
        *g->players[i]=*g->defaultPlayer;
        cl->clientGame=g;
        return 1;
    }

}

void destroyGame(game *g) {
    if (g != NULL) {
        free(g);
    }
}