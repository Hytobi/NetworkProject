//
// Created by hana on 10/12/23.
//

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#define SOL " "
#define MUR "#"
#define MUR_INCA "X"
#define VIDE "/"
#define CLASSIC_BOMB "B"
#define REMOTE_BOMB "R"
#define MINE "M"
#define ITEM "I"
#define PLAYER "@"

#include "struct.h"

void setMapInfo(Maps *mapInfo);

/**
 * retourne la map associe à l'id donnée en paramètre,
 * si elle n'a pas ete trouve return NULL
 * @param mapListe
 * @param id
 * @return
 */
Map * getMap(Maps * mapListe, int id);

int nextPosX(int i, int mapId);
int nextPosY(int i, int mapId);

#endif //PROJECT_MAP_H
