//
// Created by hana on 20/12/23.
//

#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include "bombe.h"
#include "map.h"
#include "cJSON/cJSON.h"
#include "json.h"
#include "err.h"

#define isBombe(carac) (carac==CLASSIC_BOMB_CHAR || carac==REMOTE_BOMB_CHAR || carac==MINE_CHAR)

#define postAttackExplose "POST attack/explose"
#define POST_ATTACK_EXPLOSE_SIZE strlen(postAttackExplose)

void *bombeThreadExplose(void *arg) {
    Game *game = (Game *) arg;

    sleep(3);  // Attendre 3 secondes

    Bombe *b = game->bombesListe->bombes[0];

    pthread_mutex_lock(&(game->bombesListe->mutex));

    int i = 1;
    while (b != NULL && i < MAX_BOMBES) {
        b = game->bombesListe->bombes[i];
        i++;
    }

    // Récupérer la prochaine bombe à exploser
    for (; i < MAX_BOMBES; i++) {
        Bombe *nextBomb = game->bombesListe->bombes[i];
        if (nextBomb == NULL) {
            continue;
        }
        if (b->id > nextBomb->id) {
            b = nextBomb;
        }
    }

    if (b==NULL){
        pthread_mutex_unlock(&(game->bombesListe->mutex));
        pthread_exit(NULL);
    }


    //faire exploser la bombe
    processExploseDist(game, b->x, b->y, b->dist, "classic");

    b->nbBombes--;

    destroyBombe(game->bombesListe->bombes[i]);

    pthread_mutex_unlock(&(game->bombesListe->mutex));

    pthread_exit(NULL);
}

int sendModifiedMap(Game *g, Bombe *propagation, int nbItem, int x, int y, int dist, char* type) {
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int n;
    cJSON *json = sendModifMap(g, propagation, nbItem, x, y, dist, type);
    sprintf(buffer, "%s\n%s", postAttackExplose, cJSON_Print(json));

    for (int i=0;i<MAX_PLAYER;i++){
        Player *player = g->players[i];
        if (!player){
            continue;
        }
        socklen_t clientAddrLen = sizeof(player->addr);
        n = (int) sendto(player->socket, buffer, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &player->addr, clientAddrLen);
        if (n == ERR) {
            perror("Erreur envoie du message");
            return ERR;
        }
    }
    free(buffer);
}

char getRandomChar() {
    srand(time(NULL));
    int randomNumber = (rand() % 100) + 1;
    return (randomNumber < 30) ? ITEM_CHAR : SOL_CHAR;
}

int processExplose(Game *g, int x, int y) {
    Map *map = g->map;
    int numCase = y + map->width * x;
    char carac = map->content[numCase];
    if (carac == MUR_CHAR) {
        map->content[numCase] = getRandomChar();
        return (map->content[numCase] == ITEM_CHAR) ? 2 : 1;
    } else if (carac == MUR_INCA_CHAR || isBombe(carac)) {
        return 1;
    } else if (carac == PLAYER_REMOTE_BOMB_CHAR || carac == PLAYER_BOMB_CHAR ) {
        for (int i = 0; i < MAX_PLAYER; i++) {
            Player *p = g->players[i];
            if (p == NULL) {
                continue;
            }
            if (p->x == x && p->y == y) {
                p->life -= 30;
                if (p->life <= 0) {
                    p->life = 0;
                    map->content[numCase] = SOL_CHAR;
                    //TODO : suppression de joueur
                } else {
                    map->content[numCase] = PLAYER_CHAR;
                }
                return 1;
            }
        }
    }
    return 0;
}

int processExploseDist(Game *g, int x, int y, int dist, char* type) {
    Map *map = g->map;
    int explosion;
    int nbItem = 0;
    pthread_mutex_t mutex;
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Erreur initialisation du mutex");
        exit(2);
    }

    pthread_mutex_lock(&mutex);
    printf("dist : %d\n", dist);
    Bombe propagation[5];
    // La case de la bombe explose et devient un sol
    int numCase = y + map->width * x;
    char* carac = map->content[numCase];
    if (carac == PLAYER_REMOTE_BOMB_CHAR || carac == PLAYER_BOMB_CHAR){
        //TODO : prendre degat
        map->content[numCase] = PLAYER_CHAR;
    } else {
        map->content[numCase] = SOL_CHAR;
    }

    // Les cases autour de la bombe explosent
    for (int i = 1; i <= dist; i++) {
        if (x - i > 0) {
            if (explosion = processExplose(g, x - i, y)) {
                if (explosion == 2) {
                    propagation[nbItem].x = x - i;
                    propagation[nbItem].y = y;
                    nbItem++;
                }
                break;
            }
        }
    }
    for (int i = 1; i <= dist; i++) {
        if (x + i < map->width-1) {
            if (explosion = processExplose(g, x + i, y)) {
                if (explosion == 2) {
                    propagation[nbItem].x = x - i;
                    propagation[nbItem].y = y;
                    nbItem++;
                }
                break;
            }
        }
    }
    for (int i = 1; i <= dist; i++) {
        if (y - i > 0) {
            if (explosion = processExplose(g, x, y - i)) {
                if (explosion == 2) {
                    propagation[nbItem].x = x - i;
                    propagation[nbItem].y = y;
                    nbItem++;
                }
                break;
            }
        }
    }
    for (int i = 1; i <= dist; i++) {
        if (y + i < map->height-1) {
            if (explosion = processExplose(g, x, y + i)) {
                if (explosion == 2) {
                    propagation[nbItem].x = x - i;
                    propagation[nbItem].y = y;
                    nbItem++;
                }
                break;
            }
        }
    }
    sendModifiedMap(g, propagation, nbItem, x, y, dist, type);
    pthread_mutex_unlock(&mutex);
}

int exploseBomb(Game *g, Player *p) {
    Map *map = g->map;
    int i = 0;
    while (i < p->nbRemoteBombSet) {
        int x = p->remoteSet[i].x;
        int y = p->remoteSet[i].y;
        int numCase = y + map->width * x;
        char carac = map->content[numCase];
        if (carac == REMOTE_BOMB_CHAR || carac == PLAYER_REMOTE_BOMB_CHAR) {
            processExploseDist(g, x, y, p->remoteSet[i].dist, "remoteBomb");
            p->nbRemoteBombSet--;
            p->remoteSet[i] = p->remoteSet[p->nbRemoteBombSet];
            p->remoteSet[p->nbRemoteBombSet].x = 0;
            p->remoteSet[p->nbRemoteBombSet].y = 0;
            i--;
        }
        i++;
    }
    return 1;
}

int createBombe(Bombes *bombesInfo, int x, int y, int dist, int *nbBombe) {
    Bombe *b;
    for (int i = 0; i < MAX_BOMBES; i++) {
        if ((b = bombesInfo->bombes[i]) == NULL) {
            continue;
        }

        b = malloc(sizeof(Bombe));
        if (b == NULL) {
            perror("Erreur malloc Bombe");
            return ERR;
        }
        b->x=x;
        b->y=y;
        b->dist=dist;
        b->nbBombes=nbBombe;
        b->id=bombesInfo->nextId;
        return b->id;

    }
    return ERR;
}

void destroyBombe(Bombe *b) {
    if (b!=NULL){
        free(b);
    }
}