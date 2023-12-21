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

void setMapInfo(Maps *mapInfo);

/**
 * retourne la map associe à l'id donnée en paramètre,
 * si elle n'a pas ete trouve return NULL
 * @param mapListe
 * @param id
 * @return
 */
Map *getMap(Maps *mapListe, int id);

void afficheMap(Map m);

int nextPosX(int i, int mapId);
int nextPosY(int i, int mapId);

void destroyMap(Map *);

#endif // PROJECT_MAP_H
