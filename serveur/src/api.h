/** @author: PLOUVIN Patrice
 * @date: 24/11/2022
 * @brief: description
 * @file: src/example.c
 */

#ifndef API_H
#define API_H

// path /scan : le client envoie un message broadcast pour trouver les serveurs
char* searchServer();

// path /connect : le client se connecte au serveur
void connectToServer();

// path /map : le client demande la liste des maps
void askExistingMap(char* ip);


// path /map/{id} : le client demande les jeux en cours sur la map {id}
void askGameForMap(char* ip, int mapId);

// path /map/enter/{id} : le client entre dans la partie {id}
void enterInGame(char* ip, int mapId);

// path /map/create/{id} : le client crée une partie sur la map {id}
void createGame(char* ip, int mapId);


#endif // API_H

