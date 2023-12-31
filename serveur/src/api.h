/**
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

#ifndef PROJECT_API_H
#define PROJECT_API_H

#include "cJSON/cJSON.h"
#include <string.h>
#include "map.h"

#define messageClientAttendue "looking for bomberstudent servers"
#define notifClientServeurUp "hello i'm a bomberstudent server"
#define reboot "I was connect to you"

#define getPartieListe "GET game/list"
#define GET_PARTIE_LISTE_SIZE strlen(getPartieListe)

#define getMapListe "GET Maps/list"
#define GET_MAP_LISTE_SIZE strlen(getMapListe)

#define postCreateGame "POST game/create"
#define POST_CREATE_GAME_SIZE strlen(postCreateGame)

#define postJoinGame "POST game/join"
#define POST_JOIN_GAME_SIZE strlen(postJoinGame)

#define postPlayerMove "POST player/move"
#define POST_PLAYER_MOVE_SIZE strlen(postPlayerMove)

#define postPlayerAttack "POST player/attack"
#define POST_PLAYER_ATTACK_SIZE strlen(postPlayerAttack)

#define postAttackRemoteGo "POST attack/remote/go"
#define POST_ATTACK_REMOTE_GO_SIZE strlen(postAttackRemoteGo)

#define postObjectNew "POST object/new"
#define POST_OBJECT_NEW_SIZE strlen(postObjectNew)

#define POST_POSITION_PLAYER_UPDATE "POST player/position/update"
#define POST_POSITION_PLAYER_UPDATE_SIZE strlen(POST_POSITION_PLAYER_UPDATE)

#define postAttackNewbomb "POST attack/newbomb"
#define POST_ATTACK_NEWBOMB_SIZE strlen(POST_ATTACK_NEWBOMB)

#define postAttackAffect "POST attack/affect"

#define attackMineExplose "POST attack/mineExplose"

#define POST_NEW_PLAYER "POST player/new"

#define CLASSIC "classic"
#define CLASSIC_SIZE strlen(CLASSIC)

#define POST_UPDATE_PLAYER_NB_BOMBE "POST update/player/nbBombe"
#define POST_UPDATE_PLAYER_NB_BOMBE_SIZE strlen(POST_UPDATE_PLAYER_NB_BOMBE)

/**
 * Fonction qui gère la communication avec le client
 * @param args structure contenant toutes les infos necessaire
 */
void *clientCommunication(void *args);

#endif // PROJECT_API_H
