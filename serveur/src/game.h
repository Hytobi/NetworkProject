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
int createGame(Games *gameInfo, Maps *maps, cJSON *info, Client *cl);

/**
 * Fais rejoindre la game au Client choisis,
 * return l'indice du joueur dans le tableau si tous s'est bien passe,
 * ERR sinon
 */
int joinGame(Game *g, Client *cl, Map *m);

/**
 * Move le player donne en parametre dans la game donne en parametre
 * Return le num de la case si tous se passe bien
 * ERR sinon
 * @param p
 * @param game
 * @param info
 * @return
 */
int movePlayer(Player *p, Game *game, cJSON *info);

int attackPlayer(Player* p , Game* g, cJSON* info);

void destroyGame(Game *g);

#endif //PROJECT_GAME_H
