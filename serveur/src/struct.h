//
// Created by hana on 06/12/23.
//

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <pthread.h>
#include "netinet/in.h"

#define MAX_PLAYER 4
#define LIFE 100
#define SPEED 75 // en pixels par seconde
#define DEFAUT_IMPATC_DIST 2;
#define NB_CLASSIC_BOMB 2
#define NB_REMOTE_BOMB 1
#define NB_MINE 1
#define INVINCIBLE 0

#define MAX_MAP 16
#define MAX_MAP_SIZE 2048

#define SOL " "
#define MUR "#"
#define MUR_INCA "X"
#define VIDE "/"
#define CLASSIC_BOMB "B"
#define REMOTE_BOMB "R"
#define MINE "M"
#define ITEM "I"
#define PLAYER "@"

#define BUFFER_SIZE 4096
#define MAX_CLIENTS 2048
#define PORT 42069

#define STRING_SIZE 256
#define MAX_GAMES 16

typedef struct player {
    int id; /**< player id */
    int x; /**< pos x du joueur */
    int y; /**< pos y du joueur */
    struct sockaddr_in addr; /**< adresse du player */
    int life; /**< nombre de points de vie du joueur */
    int speed; /**< vitesse du joueur (1 correspond à la vitesse de base) */
    int direction; /**< TODO, jsp a quoi ca sert ? */
    int nbClassicBomb; /**< nombre de classic bombs possédées par le joueur */
    int nbRemoteBomb; /**< nombre de remote bombs possédées par le joueur */
    int nbMine; /**< nombre de mines possédées par le joueur */
    int impactDist; /**< nombre de cases impactées par une explosion (pour une direction donnée) */
    int invincible; /**<  indique si le joueur est en mode invincible */
} player;

typedef struct game {
    char name[STRING_SIZE];
    int nbPlayers; /**< nombre de joueurs actuel dans la partie */
    int mapId; /**< id de la map sur laquelle se joue la partie */
    int startPos[2]; /**< position initiale d’un nouveau joueur */
    player *defaultPlayer; /**< objet JSON décrivant les caractéristiques d’un nouveau joueur */
    player *players[MAX_PLAYER]; /**< Joueurs de la partie */
} game;

typedef struct games {
    int nbGames;
    game *gameListe[MAX_GAMES];
} games;

typedef struct client {
    int socket;
    struct sockaddr_in addr;
    int connecter; /**< 0: pas connecte, 1: connecte, 2: en cours de connection */
    game *clientGame; /**< game dans laquelle se trouve le client */
} client;

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

typedef struct client_map_games {
    client *cl;
    maps *mapInfo;
    games *gameInfo;
} client_map_games;

typedef struct thread_Info {
    client clients[MAX_CLIENTS];
    maps *mapInfo;
    pthread_mutex_t mutex;
} thread_Info;

#endif //PROJECT_STRUCT_H
