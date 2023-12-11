//
// Created by hana on 10/12/23.
//

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

#define DIR_PATH "../map"

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