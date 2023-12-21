/** @author: PLOUVIN Patrice, DELCOURT Hana
 * @date: 18/10/2023
 * @brief: Fonction principale du programme
 * @file: src/main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "struct.h"
#include "api.h"
#include "err.h"
#include "cJSON/cJSON.h"
#include "map.h"
#include "game.h"
#include "json.h"

/*
void serveurSIGINT(int signal, siginfo_t *info, void *context) {
    AllStruct *allStruc = (AllStruct *)info->si_value.sival_ptr;
    free(allStruc);
    exit(EXIT_SUCCESS);
}
*/

/**
 * Fonction à lancer dans un thread, tourne en boucle pour ecouter les messages recu sur le port 42069\n
 * afin de répondre aux clients qui cherchent un serveur et leurs répondre.
 * Si le message recu est le bon, indique que ce client souhaite potentiellement ce connecte pour le thread TCP
 * @param args
 * @return
 */
void *serveurUdp(void *args) {
    Thread_Info *threadInfo = (Thread_Info *) args;
    printf("\033[H\033[2J");

    // Création du socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    EXIT_IF_FAIL(sockfd, "Probleme creation socket");

    // Configurer le socket pour qu'il puisse être réutilisé
    EXIT_IF_FAIL(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)), "problem setsockopt");

    //  il faut attacher le socket à un port Internet et une adresse IP
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    // addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    EXIT_IF_FAIL(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)), "Probleme bind");
    EXIT_IF_FAIL(fcntl(sockfd, F_SETFL, MSG_WAITALL), "Probleme fcntl");

    // Création du socket TCP pour écouter les connexions
    int tcpListenFd = socket(AF_INET, SOCK_STREAM, 0);
    EXIT_IF_FAIL(tcpListenFd, "Erreur création du socket TCP");

    // Configuration pour l'écoute
    struct sockaddr_in tcpListenAddr;
    memset(&tcpListenAddr, 0, sizeof(tcpListenAddr));
    tcpListenAddr.sin_family = AF_INET;
    tcpListenAddr.sin_port = htons(PORT);
    tcpListenAddr.sin_addr.s_addr = INADDR_ANY;

    EXIT_IF_FAIL(bind(tcpListenFd, (struct sockaddr *) &tcpListenAddr, sizeof(tcpListenAddr)), "Probleme bind TCP");
    EXIT_IF_FAIL(listen(tcpListenFd, 5), "Probleme listen");

    printf("Serveur up\n");

    char buffer[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int n = 0;
    for (;;) {
        // adresse du client
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        // lis un message
        n = (int) recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;
        }
        buffer[n] = '\0';

        printf("Tentative de connexion de %s\n", inet_ntoa(clientAddr.sin_addr));

        // Test que le message est correct
        if (strcmp(buffer, messageClientAttendue)) {
            // Si le message n'est pas le bon, on retourne au début de la boucle
            if (strcmp(buffer, reboot)) {
                printf("Refusé !\n");
                printf("Message non reconnue : %s\n", buffer);
                sprintf(buffer2, "%s", cJSON_Print(badRequest()));
                n = (int) sendto(sockfd, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &clientAddr,
                                 clientAddrLen);
                CONTINUE_IF_FAIL(n, "Erreur envoie du message");
                continue;
            } else {
                printf("J'ai été connecté à toi !\n");
            }
        }

        printf("Message reçu de %s : %s\nMessage accepté !\n", inet_ntoa(clientAddr.sin_addr), buffer);

        // envoie du message indiquant que je suis un server pour le bomber student
        sprintf(buffer2, "%s", notifClientServeurUp);
        // sprintf(buffer2,"yes");
        n = (int) sendto(sockfd, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &clientAddr, clientAddrLen);
        CONTINUE_IF_FAIL(n, "Erreur envoie du message");

        // Block le mutex
        pthread_mutex_lock(&(threadInfo->mutex));

        // Préparation de la connection tcpConnect
        for (int i = 0; i < MAX_CLIENTS; i++) {
            //si e client est déjà connecte, il ne se reconnecte pas
            if (threadInfo->clients[i].connecter) {
                continue;
            }
            // Ajouter le nouveau Client à la structure
            // memcpy(&(threadInfo->clients[i].addr), &clientAddr, sizeof(clientAddr));
            threadInfo->clients[i].addr = clientAddr;
            threadInfo->clients[i].connecter = 2;
            threadInfo->clients[i].socket = tcpListenFd;
            break;
        }

        pthread_mutex_unlock(&threadInfo->mutex);

        // n = send(sockfd, buffer2, 1024, MSG_CONFIRM);
        EXIT_IF_FAIL(n, "Probleme send");
    }
}

/**
 * fonction à lancer dans un thread permettant de set les infos pour la connection TCP\n
 * Il s'agit d'un thread qui tourne en boucle et cherche les client qui tentes de se connecter pour ouvrir une socket TCP\n
 * associe.
 * @param args
 * @return
 */
void *tcpConnect(void *args) {
    Thread_Info *threadInfo = (Thread_Info *) args;

    // Initialisation du thread TCP
    Games *gameInfo;
    gameInfo = malloc(sizeof(Games));
    if (gameInfo == NULL) {
        perror("Erreur allocation gameInfo");
        exit(2);
    }

    // si il y a une erreur on arrete tout car rien ne marchera
    if (pthread_mutex_init(&(gameInfo->mutex), NULL) != 0) {
        perror("Erreur initialisation du mutex");
        exit(2);
    }

    for (int i = 0; i < MAX_GAMES; i++) {
        gameInfo->gameListe[i] = NULL;
    }
    gameInfo->nbGames = 0;

    pthread_t clientThreads[MAX_CLIENTS];
    int threadCount = 0;
    for (;;) {
        // lock le mutex
        pthread_mutex_lock(&threadInfo->mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            // Premiere connection du Client
            if (threadInfo->clients[i].connecter == 2) {
                // adresse TCP du Client
                socklen_t tcpClientAddrLen = sizeof(threadInfo->clients[i].addr);

                // socket TCP
                printf("Écoute sur le socket TCP...\n");
                listen(threadInfo->clients[i].socket, 1);
                int tcpFd = accept(threadInfo->clients[i].socket, (struct sockaddr *) &threadInfo->clients[i].addr,
                                   &tcpClientAddrLen);
                if (tcpFd == ERR) {
                    perror("Erreur écoute du socket TCP");
                    pthread_mutex_unlock(&threadInfo->mutex);
                    // TODO le retirer de la liste
                    continue;
                }

                // On set les infos du client qui s'est connecte
                printf("Nouveau Client connecté via TCP depuis %s\n", inet_ntoa(threadInfo->clients[i].addr.sin_addr));
                threadInfo->clients[i].connecter = 1;
                threadInfo->clients[i].socket = tcpFd;

                Client_Map_Games *cm = malloc(sizeof(Client_Map_Games));
                cm->gameInfo = gameInfo;
                cm->cl = &threadInfo->clients[i];
                cm->mapInfo = threadInfo->mapInfo;

                // on lance le thread spécifique du client
                pthread_create(&clientThreads[threadCount], NULL, clientCommunication, cm);
                threadCount++;
            }
        }
        pthread_mutex_unlock(&threadInfo->mutex);
        pthread_join(clientThreads[threadCount], NULL);
    }
    pthread_exit(NULL);
}

/**
 * Fonction principale permettant d'initialiser les premieres structure et lancer les thread UDP et TCP
 * @param arvc
 * @param argv
 * @return
 */
int main(int arvc, char **argv) {
    Thread_Info *threadInfo = malloc(sizeof(Thread_Info));

    threadInfo->mapInfo = malloc(sizeof(Maps));
    setMapInfo(threadInfo->mapInfo);

    if (pthread_mutex_init(&(threadInfo->mutex), NULL) != 0) {
        perror("Erreur initialisation du mutex");
        exit(2);
    }

    pthread_t daemon, tcpThread;

    pthread_create(&daemon, NULL, serveurUdp, threadInfo);
    pthread_create(&tcpThread, NULL, tcpConnect, threadInfo);

    pthread_join(daemon, NULL);
    pthread_join(tcpThread, NULL);

    EXIT_FAILURE;
}