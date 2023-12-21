/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include "struct.h"
#include "cJSON/cJSON.h"

/**
 * Initialise la structure Games
 * @param gameInfo
 */
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
 * Gère le déplacement du joueur
 * Return le num de la case si tous se passe bien
 * ERR sinon
 * @param p
 * @param game
 * @param info
 * @return
 */
int movePlayer(Player *p, Game *game, cJSON *info);

/**
 * Fais poser une bombe sur la map
 * Return 0 si tous se passe bien
 * ERR sinon
 * @param p
 * @param game
 * @param info
 * @return
 */
int attackPlayer(Player *p, Game *g, cJSON *info);

/**
 * Met a jour les informations du joueur
 * Return 0 si tous se passe bien
 * ERR sinon
 * @param p
 * @param game
 * @param info
 * @return
 */
int updatePlayer(Player *p, cJSON *info);

/**
 * Supprime la game
 * return 0 si tous s'est bien passe,
 * ERR sinon
 */
void destroyGame(Game *g);

#endif // PROJECT_GAME_H
