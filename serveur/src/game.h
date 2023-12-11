//
// Created by hana on 11/12/23.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include "struct.h"
#include "cJSON/cJSON.h"

void initGames(games *gameInfo);

/**
 * créer la game, et renvoie l'index à laquelle la game est stocke dans la structure
 * ERR s'il y a eu un problème
 * @param gameInfo
 * @param info
 * @return
 */
int createGame(games *gameInfo, cJSON *info);

/**
 * Fais rejoindre la game au client choisis,
 * return 1 si tous s'est bien passe,
 * ERR si erreur
 */
int joinGame(game *g, client *cl);

void destroyGame(game *g);

#endif //PROJECT_GAME_H
