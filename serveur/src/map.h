/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#define SOL_CHAR ' '
#define MUR_CHAR '#'
#define MUR_INCA_CHAR 'X'
#define VIDE_CHAR '/'
#define CLASSIC_BOMB_CHAR 'B'
#define REMOTE_BOMB_CHAR 'R'
#define MINE_CHAR 'M'
#define ITEM_CHAR 'I'
#define PLAYER_CHAR '@'

#define PLAYER_MINE_CHAR '-'
#define PLAYER_BOMB_CHAR '('
#define PLAYER_REMOTE_BOMB_CHAR ')'

#include "struct.h"

/**
 * Set les infos des maps par défaut du serveur en lisant de manière dynamique les maps présentes dans le répoertoire map
 * @param mapInfo pointeur vers la lsite des maps pour les rendres accessible a tous les clients qui se connecteront
 */
void setMapInfo(Maps *mapInfo);

/**
 * retourne la map associe à l'id donnée en paramètre,
 * si elle n'a pas ete trouve return NULL
 * @param mapListe
 * @param id
 * @return
 */
Map *getMap(Maps *mapListe, int id);

/**
 * Affiche la map donné en paramètre
 * @param m
 */
void afficheMap(Map m);

/**
 * Récupère la prochaine position X de spawn du joueur sur la map selectione
 * @param i le numéro du prochain joueur
 * @param mapId l'ide de la map
 * @return
 */
int nextPosX(int i, int mapId);

/**
 * Récupère la prochaine position Y de spawn du joueur sur la map selectione
 * @param i le numéro du prochain joueur
 * @param mapId l'ide de la map
 * @return
 */
int nextPosY(int i, int mapId);

/**
 * Détruit la map donné en paramètre
 */
void destroyMap(Map *);

#endif // PROJECT_MAP_H
