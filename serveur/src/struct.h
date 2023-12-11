//
// Created by hana on 06/12/23.
//

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <pthread.h>
#include "netinet/in.h"
#include "player.h"
#include "map.h"

#define BUFFER_SIZE 4096
#define MAX_SERVERS 8
#define MAX_CLIENTS 2048
#define PORT 42069
#define STRING_SIZE 256

typedef struct game {
    char action[STRING_SIZE]; /**< action appelée par le client */
    int statut; /**< code de retour du serveur */
    char message[STRING_SIZE]; /**< précisant le retour (ok si tout va bien) */
    int nbPlayers; /**< nombre de joueurs actuel dans la partie */
    int startPos[2]; /**< position initiale d’un nouveau joueur */
    player defaultPlayer; /**< objet JSON décrivant les caractéristiques d’un nouveau joueur */
    player players[MAX_PLAYER]; /**< Joueurs de la partie */
} game;

typedef struct client {
    int socket;
    struct sockaddr_in addr;
    int connecter; /**< 0: pas connecte, 1: connecte, 2: en cours de connection */
    game clientGame;
} client;

typedef struct client_map {
    client *cl;
    maps *mapInfo;
} client_map;

typedef struct thread_Info {
    client clients[MAX_CLIENTS];
    maps *mapInfo;
    pthread_mutex_t mutex;
} thread_Info;

#endif //PROJECT_STRUCT_H
