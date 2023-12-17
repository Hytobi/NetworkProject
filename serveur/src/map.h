//
// Created by hana on 10/12/23.
//

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#include "struct.h"

void setMapInfo(maps *mapInfo);

/**
 * retourne la map associe à l'id donnée en paramètre,
 * si elle n'a pas ete trouve return NULL
 * @param mapListe
 * @param id
 * @return
 */
map * getMap(maps * mapListe, int id);

int nextPosX(int i, int mapId);
int nextPosY(int i, int mapId);

#endif //PROJECT_MAP_H
