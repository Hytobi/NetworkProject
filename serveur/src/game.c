//
// Created by hana on 11/12/23.
//

#include <malloc.h>
#include <string.h>
#include "game.h"
#include "stdio.h"
#include "err.h"
#include "player.h"
#include "map.h"
#include "json.h"
#include "api.h"

#define TEST_MOVES(carac) (carac!=MUR_INCA_CHAR && carac!=MUR_CHAR && carac!=VIDE_CHAR && carac!=PLAYER_CHAR && carac!=CLASSIC_BOMB_CHAR && carac!=REMOTE_BOMB_CHAR \
&& carac!=PLAYER_BOMB_CHAR && carac!=PLAYER_REMOTE_BOMB_CHAR && carac!=PLAYER_MINE_CHAR)

int createGame(Games *gameInfo, Maps *maps, cJSON *info, Client *cl) {
    int i = 0;
    while (i < MAX_GAMES) {
        if (gameInfo->gameListe[i] == NULL) {
            break;
        }
        i++;
    }
    if (i >= MAX_GAMES) {
        printf("Impossible de créer une nouvelle game, nombre maximum atteint !\n");
        return ERR;
    }
    Game *g;
    g = malloc(sizeof(Game));
    if (g == NULL) {
        perror("Erreur allocation mémoire de la partie");
        return ERR;
    }
    strcpy(g->name, cJSON_GetObjectItemCaseSensitive(info, "name")->valuestring);
    g->nbPlayers = 0;
    g->mapId = cJSON_GetObjectItemCaseSensitive(info, "mapId")->valueint;
    g->map = malloc(sizeof(Map));
    if (g->map == NULL) {
        perror("erreur malloc map in createGame");
        return ERR;
    }
    pthread_mutex_lock(&maps->mutex);
    *g->map = *getMap(maps, g->mapId);
    pthread_mutex_unlock(&maps->mutex);
    g->defaultPlayer = createPlayer(0, 1, 1, cl->addr, cl->socket);
    g->startPos[0] = 1;
    g->startPos[1] = 1;
    if (pthread_mutex_init(&(g->mutex), NULL) != 0) {
        perror("Erreur initialisation du mutex");
        exit(2);
    }

    for (int i=0;i<MAX_PLAYER;i++){
        g->players[i]=NULL;
    }

    gameInfo->gameListe[i] = g;
    gameInfo->nbGames++;
    return i;
}

int joinGame(Game *g, Client *cl, Map *m) {
    if (g->nbPlayers > MAX_PLAYER) {
        printf("Impossible de rejoindre, partie pleine\n");
        return ERR;
    }
    int i = 0;
    while (i <= MAX_PLAYER) {
        if (g->players[i] != NULL) {
            i++;
        }
        g->nbPlayers++;
        g->players[i] = malloc(sizeof(Player));
        if (g->players[i] == NULL) {
            perror("Allocation joueur de la partie");
            return ERR;
        }
        *g->players[i] = *g->defaultPlayer;
        g->players[i]->id = g->nbPlayers;
        g->players[i]->x=g->defaultPlayer->x;
        g->players[i]->y=g->defaultPlayer->y;

        pthread_mutex_lock(&m->mutex);
        m->content[g->players[i]->y + m->width * g->players[i]->x] = '@'; // place le joueur sur la map
        pthread_mutex_unlock(&m->mutex);

        g->defaultPlayer->x = nextPosX(i, g->mapId);
        g->defaultPlayer->y = nextPosY(i, g->mapId);
        cl->clientGame = g;
        cl->player = g->players[i];
        return i;
    }
    return ERR;
}

/**
 * Si le joueur marche sur une mine il doit prendre des dégats et la map doit s'update
 * @param p
 * @param m
 * @return
 */
void moveOnMine(Player *p, Map *m) {
    if (p->invincible) {
        return;
    }
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    p->life -= 30;
    int n = ERR;
    int i = 0;
    do {
        sprintf(buffer, "%s", sendAttackAffect(p));
        socklen_t clientAddrLen = sizeof(p->addr);
        n = (int) sendto(p->socket, buffer, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &p->addr, clientAddrLen);
        i++;
    } while (n == ERR && i < 50); // essaye d'envoyer jusqu'a 50 fois si ca marche pas
    free(buffer);
}

char moveAfterAttack(char maCase) {
    if (maCase == PLAYER_MINE_CHAR) {
        return MINE_CHAR;
    } else if (maCase == PLAYER_REMOTE_BOMB_CHAR) {
        return REMOTE_BOMB_CHAR;
    } else if (maCase == PLAYER_BOMB_CHAR) {
        return CLASSIC_BOMB_CHAR;
    }
    return SOL_CHAR;
}

void envoieMineExplose(Game *g, int x, int y, Player *p) {
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int n;
    cJSON *json = cJSON_CreateObject();
    char pos[8];
    sprintf(pos, "%d,%d", x, y);
    cJSON_AddStringToObject(json, "pos", pos);
    sprintf(buffer, "%s\n%s", attackMineExplose, cJSON_Print(json));
    socklen_t clientAddrLen = sizeof(p->addr);
    n = (int) sendto(p->socket, buffer, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &p->addr, clientAddrLen);
    if (n == ERR) {
        perror("Erreur envoie du message");
        return;
    }


    /* TODO
    for (int i=0; i<MAX_PLAYER; i++){

        if (g->players[i]!=NULL){

            printf("Envoie de %s\n", buffer);
            printf("addr : %s\n", inet_ntoa(g->players[i]->addr.sin_addr));
            socklen_t clientAddrLen = sizeof(g->players[i]->addr);
            n = (int) sendto(g->players[i]->socket, buffer, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &g->players[i]->addr, clientAddrLen);
            if (n == ERR) {
                perror("Erreur envoie du message");
                return;
            }
        }
    }*/
    free(buffer);
}

int movePlayer(Player *p, Game *game, cJSON *info) {
    Map *map = game->map;
    char move[5];
    strcpy(move, cJSON_GetObjectItemCaseSensitive(info, "move")->valuestring);
    int actual_x = p->x, actual_y = p->y;
    int numCase = actual_y + map->width * actual_x;
    // si le joueur n'a pas la bonne place sur la map
    if (map->content[numCase] != PLAYER_CHAR) {
        //TODO
    }
    char carac;
    // up
    if (!strcmp(move, "up")) {
        int nextCase = numCase - map->width;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                envoieMineExplose(game, p->x - 1, p->y, p);
                //return numCase * map->width;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->x--;
            return nextCase;
        }
        // down
    } else if (!strcmp(move, "down")) {
        int nextCase = numCase + map->width;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                envoieMineExplose(game, p->x + 1, p->y, p);
                //return numCase + map->width;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->x++;
            return nextCase;
        }
        // left
    } else if (!strcmp(move, "left")) {
        int nextCase = numCase - 1;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                envoieMineExplose(game, p->x, p->y - 1, p);
                //return numCase - 1;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->y--;
            return nextCase;
        }
        // right
    } else {
        int nextCase = numCase + 1;
        carac = map->content[nextCase];
        if (TEST_MOVES(carac)) {
            if (carac == MINE_CHAR) {
                moveOnMine(p, map);
                envoieMineExplose(game, p->x, p->y + 1, p);
                //return numCase + 1;
            }
            map->content[numCase] = moveAfterAttack(map->content[numCase]);
            map->content[nextCase] = PLAYER_CHAR;
            p->y++;
            return nextCase;
        }
    }

    return ERR;
}

int attackPlayer(Player *p, Game *g, cJSON *info) {
    char move[5];
    strcpy(move, cJSON_GetObjectItemCaseSensitive(info, "pos")->valuestring);
    int x = atoi(strtok(move, ","));
    int y = atoi(strtok(NULL, ","));
    Map *map = g->map;
    int numCase = y + map->width * x;
    char carac = map->content[numCase];
    if (carac == PLAYER_CHAR) {
        // peut etre changer content en une liste de structure 'case' coup set des champ tel que "a bombe"
        char *type = cJSON_GetObjectItemCaseSensitive(info, "type")->valuestring;
        if (!strcmp(type, "classic")) {
            p->nbClassicBomb--;
            map->content[numCase] = PLAYER_BOMB_CHAR;
        } else if (!strcmp(type, "remote")) {
            p->nbRemoteBomb--;
            p->remoteSet[p->nbRemoteBombSet].x = x;
            p->remoteSet[p->nbRemoteBombSet].y = y;
            p->nbRemoteBombSet++;
            map->content[numCase] = PLAYER_REMOTE_BOMB_CHAR;
        } else if (!strcmp(type, "mine")) {
            p->nbMine--;
            map->content[numCase] = PLAYER_MINE_CHAR;
        }
        return 1;

    }
    return ERR;
}

int processExplose(Game *g, int x, int y) {
    Map *map = g->map;
    int numCase = y + map->width * x;
    char carac = map->content[numCase];
    if (carac == MUR_CHAR) {
        map->content[numCase] = SOL_CHAR;
        //todo : peut drop un item
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


void destroyGame(Game *g) {
    if (g != NULL) {
        free(g);
    }
}