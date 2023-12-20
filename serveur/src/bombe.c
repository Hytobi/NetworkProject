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

int createBombe(Bombes * bombesInfo){

}

void destroyBombe(Bombe * b){

}