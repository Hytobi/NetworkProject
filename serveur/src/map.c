//
// Created by hana on 10/12/23.
//

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "err.h"
#include "struct.h"

#define DIR_PATH "../map"

int nextX[2][MAX_PLAYER]={{1,14,1,14},{1,20,1,20}};
int nextY[2][MAX_PLAYER]={{1,1,13,13},{1,1,6,6}};

void setMapInfo(maps *mapInfo) {
    mapInfo->nbMap = 0;
    int i = 0;

    DIR *dir = opendir(DIR_PATH);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }
        if (i >= MAX_MAP) {
            break;
        }
        mapInfo->mapListe[i]= malloc(sizeof(map));
        strcpy(mapInfo->mapListe[i]->content,"");
        mapInfo->mapListe[i]->id=i;
        mapInfo->mapListe[i]->height=0;
        mapInfo->mapListe[i]->width=0;

        char filePath[PATH_MAX];
        snprintf(filePath, sizeof(filePath), "%s/%s", DIR_PATH, entry->d_name);

        // Lire le contenu du fichier
        FILE *file = fopen(filePath, "r");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_MAP_SIZE];
        while (fgets(buffer, MAX_MAP_SIZE, file) != NULL) {
            strcat(mapInfo->mapListe[i]->content, buffer);
            mapInfo->mapListe[i]->height++;
        }
        mapInfo->mapListe[i]->width= strlen(mapInfo->mapListe[i]->content)/mapInfo->mapListe[i]->height;

        fclose(file);
        i++;
    }
    mapInfo->nbMap = i;
    closedir(dir);
}

map * getMap(maps * mapListe, int id){
    map * m;
    int i=0;
    do {
        if (i>mapListe->nbMap){
            return NULL;
        }
        if (!mapListe->mapListe[i]){
            i++;
            continue;
        }
        m=mapListe->mapListe[i];
        i++;
    } while (m->id!=id);
    return m;
}

int nextPosX(int i, int mapId){
    return nextX[mapId][i];
}

int nextPosY(int i, int mapId){
    return nextY[mapId][i];
}

/*
Je trouve pas de structure pour avec les infos de la partie en cours
int movePlayer(map *map, cJSON *info){
    char *move = malloc(sizeof(char) * 7); // "(XX,XX)"
    if (move == NULL) {
        perror("Erreur allocation mémoire du mouvement");
        return ERR;
    }
    strcpy(move,cJSON_GetObjectItemCaseSensitive(info,"move")->valuestring);
    int x = atoi(strtok(move, ","));
    int y = atoi(strtok(NULL, ","));
    if (x < 0 || x > map->width || y < 0 || y > map->height) {
        // Ne peut pas se déplacer
        return 0;
    }
    char carac = map->content[x+y*map->width];
    if (!strcmp(carac,MUR_INCA) || !strcmp(carac,MUR) || !strcmp(carac,VIDE)) { // # / ou X
        // Ne peut pas se déplacer
        return 0;
    }
    return 1;
}
*/