//
// Created by hana on 10/12/23.
//

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#define MAX_MAP 16
#define MAX_MAP_SIZE 2048

typedef struct map {
    int id; /**< id de la map concernée*/
    int width; /**<largeur de la map en nombre de cases*/
    int height; /**<hauteur de la map en nombre de cases*/
    char content[MAX_MAP_SIZE]; /** descriptif de la map
 * La map est décrite au moyen des caractères suivants :
 * = correspond à un mur cassable par une explosion de bombe
 * * correspond à un mur incassable
 * - correspond à une case libre */
} map;

typedef struct maps {
    int nbMap;
    map *mapListe[MAX_MAP];
} maps;

void setMapInfo(maps *mapInfo);

#endif //PROJECT_MAP_H
