//
// Created by hana on 20/12/23.
//

#include <unistd.h>
#include <malloc.h>
#include "bombe.h"

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
        return 1;
    } else if (carac == MUR_INCA_CHAR) {
        return 1;
    } else if (carac == PLAYER_CHAR) {
        //TODO : le joueur prend des dégats
        return 1;
    } else if (carac == PLAYER_BOMB_CHAR) {
        //processExploseDist();
        return 1;
    } else if (carac == PLAYER_REMOTE_BOMB_CHAR) {
        //TODO : la bombe explose
        return 1;
    } else if (carac == PLAYER_MINE_CHAR) {
        //TODO : la mine explose
        return 1;
    }
    return 0;
}

int processExploseDist(Game *g, int x, int y, int dist) {
    Map *map = g->map;
    for (int i = 1; i <= dist; i++) {
        if (x - i >= 0) {
            if (processExplose(g, x - i, y)) {
                break;
            }
        }
        if (x + i < map->width) {
            if (processExplose(g, x + i, y)) {
                break;
            }
        }
        if (y - i >= 0) {
            if (processExplose(g, x, y - i)) {
                break;
            }
        }
        if (y + i < map->height) {
            if (processExplose(g, x, y + i)) {
                break;
            }
        }
    }
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
            map->content[numCase] = SOL_CHAR;
            processExploseDist(g, x, y, p->remoteSet[i].dist);
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