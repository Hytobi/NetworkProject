/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <pthread.h>
#include "netinet/in.h"

#define MAX_PLAYER 4
#define LIFE 100
#define SPEED 1 // 75 en pixels par seconde
#define DEFAUT_IMPATC_DIST 2;
#define NB_CLASSIC_BOMB 2
#define NB_REMOTE_BOMB 1
#define NB_MINE 1
#define INVINCIBLE 0

#define MAX_BOMBES 32

#define MAX_MAP 16
#define MAX_MAP_SIZE 2048

#define BUFFER_SIZE 4096
#define MAX_CLIENTS 2048
#define PORT 42069

#define STRING_SIZE 256
#define MAX_GAMES 16
#define MAX_REMOTE_BOMB 32
#define MOVE_INVINCIBLE 7

/**
 * Structure qui definie une bombe
 * (Elle est incassable, meme une explosion la fera pas exploser avant la fin de son timer)
 */
typedef struct Bombe {
    int id;
    int x;
    int y;
    int dist;
    int *nbBombes; /**< pointeur du nombre de bombes posées par le joueur */
    int playerId;
    int socket;
    struct sockaddr_in addr;
} Bombe;
typedef struct player {
    int id;                           /**< player id */
    int x;                            /**< pos x du joueur */
    int y;                            /**< pos y du joueur */
    struct sockaddr_in addr;          /**< adresse du player */
    int socket;                       /**< socket associe au player */
    int life;                         /**< nombre de points de vie du joueur */
    int speed;                        /**< vitesse du joueur (1 correspond à la vitesse de base) */
    int direction;                    /**< TODO, jsp a quoi ca sert ? */
    int nbClassicBomb;                /**< nombre de classic bombs possédées par le joueur */
    int nbRemoteBomb;                 /**< nombre de remote bombs possédées par le joueur */
    int nbMine;                       /**< nombre de mines possédées par le joueur */
    int impactDist;                   /**< nombre de cases impactées par une explosion (pour une direction donnée) */
    int invincible;                   /**<  indique si le joueur est en mode invincible */
    int nbMoveInvincible;             /**< nombre de déplacements restants en mode invincible */
    int nbRemoteBombSet;              /**< nombre de bombes posées par le joueur */
    Bombe remoteSet[MAX_REMOTE_BOMB]; /**< liste des positions des bombes posées par le joueur */
} Player;

typedef struct Bombes {
    int nextId;
    Bombe *bombes[MAX_BOMBES];
    pthread_mutex_t mutex;
} Bombes;

typedef struct map {
    int id;                     /**< id de la map concernée*/
    int width;                  /**<largeur de la map en nombre de cases*/
    int height;                 /**<hauteur de la map en nombre de cases*/
    char content[MAX_MAP_SIZE]; /** descriptif de la map*/
    pthread_mutex_t mutex;
} Map;

typedef struct maps {
    int nbMap;
    Map *mapListe[MAX_MAP];
    pthread_mutex_t mutex;
} Maps;

typedef struct game {
    char name[STRING_SIZE];
    int nbPlayers;               /**< nombre de joueurs actuel dans la partie */
    int mapId;                   /**< id de la map sur laquelle se joue la partie */
    Map *map;                    /**< map actuelle de la partie */
    int startPos[2];             /**< position initiale d’un nouveau joueur */
    Player *defaultPlayer;       /**< objet JSON décrivant les caractéristiques d’un nouveau joueur */
    Player *players[MAX_PLAYER]; /**< Joueurs de la partie */
    Bombes *bombesListe;
    pthread_mutex_t mutex;
} Game;

typedef struct games {
    int nbGames;
    Game *gameListe[MAX_GAMES];
    pthread_mutex_t mutex;
} Games;

typedef struct client {
    int socket;
    struct sockaddr_in addr;
    int connecter;    /**< 0: pas connecte, 1: connecte, 2: en cours de connection */
    Game *clientGame; /**< game dans laquelle se trouve le Client */
    Player *player;   /**< player associe au client */
} Client;

typedef struct client_map_games {
    Client *cl;
    Maps *mapInfo;
    Games *gameInfo;
} Client_Map_Games;

typedef struct thread_Info {
    Client clients[MAX_CLIENTS];
    Maps *mapInfo;
    pthread_mutex_t mutex;
} Thread_Info;

#endif // PROJECT_STRUCT_H
