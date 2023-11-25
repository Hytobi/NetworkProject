/** @author: PLOUVIN Patrice
 * @date: 24/11/2022
 * @brief: description
 * @file: src/example.c
 */
#include "beforegame.h"

#include <unistd.h>


void enterName() {
    printf("\033[H\033[2J");
    printf("Entrez votre nom: ");
    char name[20];
    scanf("%s", name);
    printf("Bonjour %s\n", name);
    sleep(2);
    printf("\033[H\033[2J");
}
void searchServer();
void connectToServer();
void askExistingMap();
void askGameForMap(char*);
void enterInGame();
void createGame();


