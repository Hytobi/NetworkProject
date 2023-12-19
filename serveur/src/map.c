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

void setMapInfo(Maps *mapInfo) {
    mapInfo->nbMap = 0;
    int i = 0;

    if (pthread_mutex_init(&(mapInfo->mutex), NULL) != 0) {
        perror("Erreur initialisation du mutex");
        exit(2);
    }

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
        mapInfo->mapListe[i]= malloc(sizeof(Map));
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
            size_t newlinePos = strcspn(buffer, "\n");

            // Si un saut de ligne est trouvé, le remplacer par le caractère nul
            if (buffer[newlinePos] == '\n') {
                buffer[newlinePos] = '\0';
            }

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

Map * getMap(Maps * mapListe, int id){
    Map * m;
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

void afficheMap(Map m){
    for (int i=0;i<m.height;i++){
        for (int j=0;j<m.width;j++){
            printf("%c",m.content[i*m.width+j]);
        }
        printf("\n");
    }
}

int nextPosX(int i, int mapId){
    return nextX[mapId][i];
}

int nextPosY(int i, int mapId){
    return nextY[mapId][i];
}