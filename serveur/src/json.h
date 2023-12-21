/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_JSON_H
#define PROJECT_JSON_H

#include "cJSON/cJSON.h"
#include "struct.h"

/**
 * retourne le player en format cJSON
 * @param p
 * @return
 */
cJSON *playerToJSON(Player p);

/**
 * retourne les maps en format cJSON
 * @param map
*/
cJSON *sendMapListe(Maps *mapsInfo);

/**
 * retourne la game crée en format cJSON
 * @param g
 * @return
 */
cJSON *sendGameCreation(Game *g, Map *map);

/**
 * retourne la liste des games en format cJSON
 * @param gameInfo
 * @return
 */
cJSON *sendPartieListe(Games *gameInfo);

/**
 * retourne la game rejoint par le player en format cJSON
 * @param g
 * @param p
 * @return
 */
cJSON *sendJoinGame(Game *g, Player *p);

/**
 * retourne le mouvement du joueur en format cJSON
 * @param p
 * @param move
 * @return
 */
cJSON *sendMove(Player *p, char move[5]);

/**
 * retourne la position d'une explosion de bombe en format cJSON
 * @param p
 * @return
 */
char *sendAttackAffect(Player *p);

/**
 * retourne la position d'une bombe posé en format cJSON
 * @param b
 * @param p
 * @return
 */
cJSON *sendPosBomb(cJSON *b, Player *p);

/**
 * retourne les informations du joueur qui a ramassé un objet en format cJSON
 * @param p
 * @return
 */
cJSON *sendObjNew(Player *p);

/**
 * retourne un joueur qui rejoin une partie en cours en format cJSON
 * @param p
 * @return
 */
cJSON *newPlayer(Player p);

/**
 * retourne le nombre de bombe courant d'un joueur en format cJSON
 * @param nbBomb
 * @return
 */
cJSON *sendNbBombs(int nbBomb);

/**
 * retourne la liste des case modifié apres une explosion de bombe en format cJSON
 * @param g
 * @param propagation
 * @param nbItem
 * @param x
 * @param y
 * @param dist
 * @param type
 * @return
 */
cJSON *sendModifMap(Game *g, Bombe *propagation, int nbItem, int x, int y, int dist, char *type);

/**
 * retourne une erreur bad request en format cJSON
*/
cJSON *badRequest();

/**
 * retourne une erreur inconnue en format cJSON
*/
cJSON *errInconnue();

#endif // PROJECT_JSON_H
