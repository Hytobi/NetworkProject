
#ifndef PLAYER_H
#define PLAYER_H

#define PV 100
#define VITESSE 75 // en pixels par seconde
#define DEFAULT_SIZE_EXPLOSION 2;
#define NB_DISPO_CLASSIC 2
#define NB_DISPO_REMOTE 1
#define NB_DISPO_MINE 1
#define INVINCIBLE 0

typedef struct Player Player;
struct Player {
    int id;
    int x;
    int y;
    int pv;
    int vitesse;
    int direction;
    int size_explosion;
    int nb_dispo_classic;
    int nb_dispo_remote;
    int nb_dispo_mine;
    int invincible;
};

#endif // PLAYER_H