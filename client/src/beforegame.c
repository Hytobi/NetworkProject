/** @author: PLOUVIN Patrice
 * @date: 24/11/2022
 * @brief: description
 * @file: src/example.c
 */
#include "beforegame.h"

#include "processJson.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"
#include "rage.h"
#include "api.h"


void enterName() {
    printf("\033[H\033[2J");
    printf("Entrez votre nom: ");
    char name[20];
    scanf("%s", name);
    printf("Bonjour %s\n", name);
    sleep(2);
    printf("\033[H\033[2J");
}

char* searchServer() {
    printf("\033[H\033[2J");
    printf("Recherche de serveur...\n");
    char* msgScan = getJsonScan();
    char* broadcastAddress = getBroadcastAddress();
    char* postResponse = post(broadcastAddress, msgScan);
    sleep(2);

    // on recupere la liste des serveurs dans le terminal et on les met dans une liste
    // Lit le terminal 

    char* serverList = malloc(1024 * sizeof(char));
    char* serverListToken = strtok(serverList, ",");
    while (serverListToken != NULL) {
        printf("%s\n", serverListToken);
        serverListToken = strtok(NULL, ",");
    }
    free(serverList);
    return serverList;
}

void connectToServer(char* ip) {
    printf("\033[H\033[2J");
    printf("Connexion au serveur...\n");
    char* msgConnect = getJsonConnect();
    char* postResponse = post(ip, msgConnect);
    sleep(2);
}

void askExistingMap(char* ip) {
    printf("\033[H\033[2J");
    printf("Demande de la liste des maps...\n");
    char* getResponse = get(ip, getMap());
    sleep(2);
}

void askGameForMap(char* ip, int mapId) {
    printf("\033[H\033[2J");
    printf("Demande de la map %d...\n", mapId);
    char* getResponse = get(ip, getJsonPathMap(mapId));
    sleep(2);
}

void enterInGame(char* ip, int mapId, int gameId) {
    printf("\033[H\033[2J");
    printf("Entrez dans la partie...\n");
    char* postResponse = post(ip, getJsonEnterGame(mapId, gameId));
    sleep(2);
}

void createGame(char* ip, int mapId) {
    printf("\033[H\033[2J");
    printf("Creation de la partie...\n");
    char* postResponse = post(ip,  getJsonCreateGame(mapId));
    sleep(2);
}


