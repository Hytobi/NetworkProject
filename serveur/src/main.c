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

#include "api.h"
#include "err.h"
#include "cJSON/cJSON.h"
#include "struct.h"

void *serveurUdp(void *args) {
    thread_Info *threadInfo = (thread_Info *) args;
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
    //addr.sin_port = htons(12345);
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
    int n = 0;
    for (;;) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;
        }
        buffer[n] = '\0';

        printf("Tentative de connexion de %s\n", inet_ntoa(clientAddr.sin_addr));

        if (strcmp(buffer, messageClientAttendue)) {
            printf("Refusé !\n");
            printf("Message non reconnue : %s\n", buffer);
            continue;
        }

        printf("Message reçu de %s : %s\nMessage accepté !\n", inet_ntoa(clientAddr.sin_addr), buffer);

        char buffer2[1024];
        sprintf(buffer2, "%s", notifClientServeurUp);
        //sprintf(buffer2,"yes");
        n = sendto(sockfd, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &clientAddr, clientAddrLen);
        //TODO si ca se passe mal

        // Block le mutex
        pthread_mutex_lock(&(threadInfo->mutex));

        // Préparation de la connection tcpConnect
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (threadInfo->clients[i].connecter) {
                continue;
            }
            // Ajouter le nouveau client à la structure
            // memcpy(&(threadInfo->clients[i].addr), &clientAddr, sizeof(clientAddr));
            threadInfo->clients[i].addr = clientAddr;
            threadInfo->clients[i].connecter = 2;
            threadInfo->clients[i].socket = tcpListenFd;
            break;
        }

        /*
        // Vérifier que le client n'est pas déjà connecté
        for (int i = 0; i < threadInfo->clientCount; i++) {
            if (!&threadInfo->clients[i]) {
                //TODO a retirer si ca marche sans
                printf("Client non alloue\n");
            }
            if (memcmp(&(threadInfo->clients[i].addr), &clientAddr, sizeof(clientAddr)) == 0) {
                // Ajouter le nouveau client à la structure
                threadInfo->clients[i].connecter = 2;
                memcpy(&(threadInfo->clients[threadInfo->clientCount].addr), &clientAddr,
                       sizeof(clientAddr));
                threadInfo->clientCount++;
                break;
            }
        }
         */

        pthread_mutex_unlock(&threadInfo->mutex);


        //n = send(sockfd, buffer2, 1024, MSG_CONFIRM);
        EXIT_IF_FAIL(n, "Probleme send");
    }
}

void *tcpConnect(void *args) {
    thread_Info *threadInfo = (thread_Info *) args;

    pthread_t clientThreads[MAX_CLIENTS];
    int threadCount = 0;
    for (;;) {
        pthread_mutex_lock(&threadInfo->mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            // Premiere connection du client
            if (threadInfo->clients[i].connecter == 2) {
                // adresse TCP du client
                socklen_t tcpClientAddrLen = sizeof(threadInfo->clients[i].addr);

                // socket TCP
                printf("Écoute sur le socket TCP...\n");
                listen(threadInfo->clients[i].socket,1);
                int tcpFd = accept(threadInfo->clients[i].socket, (struct sockaddr *) &threadInfo->clients[i].addr,
                                   &tcpClientAddrLen);
                if (tcpFd == ERR) {
                    perror("Erreur écoute du socket TCP");
                    pthread_mutex_unlock(&threadInfo->mutex);
                    //TODO le retirer de la liste
                    continue;
                }

                printf("Nouveau client connecté via TCP depuis %s\n", inet_ntoa(threadInfo->clients[i].addr.sin_addr));
                threadInfo->clients[i].connecter = 1;
                threadInfo->clients[i].socket=tcpFd;

                pthread_create(&clientThreads[threadCount], NULL, clientCommunication, &threadInfo->clients[i]);
                threadCount++;
            }
        }
        pthread_mutex_unlock(&threadInfo->mutex);

        for (int i = 0; i < threadCount; i++) {
            pthread_join(clientThreads[i], NULL);
        }
    }
    pthread_exit(NULL);
}

int main(int arvc, char **argv) {
    thread_Info *threadInfo = malloc(sizeof(thread_Info));
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