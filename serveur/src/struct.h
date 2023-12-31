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
    int id;     /**< id d'une bombe permettant de définir la plus vieille bombe a faire exploser */
    int x;      /**< pos x de la bombe sur la map */
    int y;      /**< pos y de la bombe sur la map */
    int dist;   /**< puissance de la bombe (nombre de case affecte par l'explosion) */
    int *nbBombes; /**< pointeur du nombre de bombes posées par le joueur */
    int playerId;   /**< id du joueur qui a pose la bombe */
    int socket;     /**< socket TCP du joueur qui a pose la bombe (afin de lui rajouter une bombe lorsqu'elle explose) */
    struct sockaddr_in addr;    /**< addr du joueur pour lui ajouter la bombe apres l'explosion */
} Bombe;

/**
 * Struct permettant de définir un Joueur
 */
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

/**
 * Liste des Classique Bombes d'une partie
 */
typedef struct Bombes {
    int nextId;     /**< Entier permettant de definir l'age d'une bombe a sa création */
    Bombe *bombes[MAX_BOMBES];  /**< Listes des bombes de la map */
    pthread_mutex_t mutex;  /**< Mutex pour acceder aux infos des bobmes */
} Bombes;

/**
 * Struct représentant une map et toutes ces infos
 */
typedef struct map {
    int id;                     /**< id de la map concernée*/
    int width;                  /**<largeur de la map en nombre de cases*/
    int height;                 /**<hauteur de la map en nombre de cases*/
    char content[MAX_MAP_SIZE]; /**< descriptif de la map*/
    pthread_mutex_t mutex;      /**< Mutex pour accéder aux infos de la map */
} Map;

/**
 * Structure représentant la liste des map par défaut
 */
typedef struct maps {
    int nbMap;  /**< Nombre de maps stocké sur le serveur */
    Map *mapListe[MAX_MAP]; /**< Liste des map par défaut */
    pthread_mutex_t mutex; /** mutex pour accéder à la liste des maps (sert surtout à l'affichage des map et a debugger) */
} Maps;

/**
 * Structure décrivant une game
 */
typedef struct game {
    char name[STRING_SIZE];     /** Nom de la game */
    int nbPlayers;               /**< nombre de joueurs actuel dans la partie */
    int mapId;                   /**< id de la map sur laquelle se joue la partie */
    Map *map;                    /**< map actuelle de la partie */
    int startPos[2];             /**< position initiale d’un nouveau joueur */
    Player *defaultPlayer;       /**< objet JSON décrivant les caractéristiques d’un nouveau joueur */
    Player *players[MAX_PLAYER]; /**< Joueurs de la partie */
    Bombes *bombesListe;        /**< Liste des bombes de la game, utilise pour savoir quelle bombe est la prochaine a exploser */
    pthread_mutex_t mutex;      /**< mutex de la game en cours */
} Game;

/**
 * Structure pour acceder au tableau de games avec tous ce qu'il faut
 */
typedef struct games {
    int nbGames; /**< Nombres de Games en cours */
    Game *gameListe[MAX_GAMES]; /**< Liste des games */
    pthread_mutex_t mutex; /** mutex pour acceder à la liste des games */
} Games;

/**
 * Structure décrivant un client
 */
typedef struct client {
    int socket; /** sokcet TCP associa a ce client */
    struct sockaddr_in addr; /** addresse du client */
    int connecter;    /**< 0: pas connecte, 1: connecte, 2: en cours de connection (pour la connection UDP puis TCP) */
    Game *clientGame; /**< game dans laquelle se trouve le Client */
    Player *player;   /**< player associe au client */
} Client;

/**
 * Structure permettant a un client d'avoir la liste des Map et des Games
 * Utile dans la gestion des messages recu
 */
typedef struct client_map_games {
    Client *cl; /** Pointeur vers le client et toutes ses infos */
    Maps *mapInfo; /**< Listee des Maps */
    Games *gameInfo; /**< Liste des Game */
} Client_Map_Games;

typedef struct thread_Info {
    Client clients[MAX_CLIENTS]; /** Tableau contenant la liste des clients */
    Maps *mapInfo; /** Listes des map par défaut */
    pthread_mutex_t mutex; /**<Mutex pour acceder a la lsite des clients lors d'une conenction */
} Thread_Info;

#endif // PROJECT_STRUCT_H
