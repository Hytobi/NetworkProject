//
// Created by hana on 11/12/23.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include "struct.h"
#include "cJSON/cJSON.h"

void initGames(Games *gameInfo);

/**
 * créer la game, et renvoie l'index à laquelle la game est stocke dans la structure
 * ERR s'il y a eu un problème
 * @param gameInfo
 * @param info
 * @return
 */
int createGame(Games *gameInfo, Maps *maps,cJSON *info, struct sockaddr_in);

/**
 * Fais rejoindre la game au Client choisis,
 * return l'indice du joueur dans le tableau si tous s'est bien passe,
 * ERR sinon
 */
int joinGame(Game *g, Client *cl, Map *m);

void destroyGame(Game *g);

#endif //PROJECT_GAME_H
