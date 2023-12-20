//
// Created by hana on 20/12/23.
//

#include <unistd.h>
#include <malloc.h>
#include "bombe.h"
#include "map.h"
#include "cJSON/cJSON.h"
#include "json.h"
#include "err.h"

#define isBombe(carac) (carac==CLASSIC_BOMB_CHAR || carac==REMOTE_BOMB_CHAR || carac==MINE_CHAR)

#define postAttackExplose "POST attack/explose"
#define POST_ATTACK_EXPLOSE_SIZE strlen(postAttackExplose)

void* bombeThread(void* arg) {
    Game* game = (Game*)arg;

    sleep(3);  // Attendre 3 secondes

    // Créer et déclencher la bombe
    Bombe* nouvelleBombe = malloc(sizeof(Bombe));
    nouvelleBombe->dist = 0;  // Valeur de distance à définir
    nouvelleBombe->x = 0;     // Valeur de coordonnée x à définir
    nouvelleBombe->y = 0;     // Valeur de coordonnée y à définir

    pthread_mutex_lock(&(game->bombesListe->mutex));
    if (game->bombesListe->nbBombe < MAX_BOMBES) {
        game->bombesListe->bombes[game->bombesListe->nbBombe] = nouvelleBombe;
        game->bombesListe->nbBombe++;
    }
    pthread_mutex_unlock(&(game->bombesListe->mutex));

    pthread_exit(NULL);
}

sendModifiedMap(Game *g, Bombe *propagation, int nbItem, int x, int y, int dist, char* type) {
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
            return;
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
    }
    return 0;
}

int processExploseDist(Game *g, int x, int y, int dist, char* type) {
    Map *map = g->map;
    int explosion;
    int nbItem = 0;
    Bombe propagation[dist*4];
    // La case de la bombe explose et devient un sol
    int numCase = y + map->width * x;
    map->content[numCase] = SOL_CHAR;

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
}

int exploseBomb(Game *g, Player *p) {
    Map *map = g->map;
    int i = 0;
    while (i < p->nbRemoteBombSet) {
        int x = p->remoteSet[i].x;
        int y = p->remoteSet[i].y;
        int numCase = y + map->width * x;
        char carac = map->content[numCase];
        if (carac == PLAYER_REMOTE_BOMB_CHAR) {
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

int createBombe(Bombes * bombesInfo){

}

void destroyBombe(Bombe * b){

}