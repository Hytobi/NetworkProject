/** @author: PLOUVIN Patrice
 * @date: 24/11/2023
 * @brief: description
 * @file: src/example.h
 */

#ifndef BEFOREGAME_H
#define BEFOREGAME_H

void enterName();
char* searchServer();
void connectToServer();
void askExistingMap();
void askGameForMap(char* ip, int mapId);
void enterInGame(char* ip, int mapId);
void createGame();


#endif