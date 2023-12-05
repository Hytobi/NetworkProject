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

#define MAX_SERVERS 8
#define MAX_CLIENTS 2048
#define BUFFER_SIZE 1024
#define PORT 42069

typedef struct game_Server {
    int socket;
    struct sockaddr_in addr;
} game_Server;

typedef struct thread_Info {
    game_Server clients[MAX_CLIENTS];
    int clientCount;
    pthread_mutex_t mutex;
} thread_Info;

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

    printf("Serveur up\n");

    char buffer[BUFFER_SIZE];
    int n = 0;
    for (;;) {
        buffer[n] = '\0';
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;
        }

        printf("Tentative de connexion de %s\n", inet_ntoa(clientAddr.sin_addr));

        if (strcmp(buffer, messageClientAttendue)) {
            printf("Refusé !\n", inet_ntoa(clientAddr.sin_addr));
            printf("Message non reconnue : %s\n", buffer);
            continue;
        }

        printf("Message reçu de %s : %s\nConnexion accepté !\n", inet_ntoa(clientAddr.sin_addr), buffer);

        char buffer2[1024];
        sprintf(buffer2, "%s\n", notifClientServeurUp);
        //sprintf(buffer2,"yes");
        n = sendto(sockfd, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &clientAddr, clientAddrLen);

        // Block le mutex
        pthread_mutex_lock(&(threadInfo->mutex));

        // Vérifier que le client n'est pas déjà connecté
        int clientIndex = -1;
        for (int i = 0; i < threadInfo->clientCount; i++) {
            if (memcmp(&(threadInfo->clients[i].addr), &clientAddr, sizeof(clientAddr)) == 0) {
                clientIndex = i;
                break;
            }
        }

        // Premiere connection du client
        if (clientIndex == -1) {
            // socket TCP
            int tcpFd = socket(AF_INET, SOCK_STREAM, 0);
            EXIT_IF_FAIL(tcpFd, "Erreur création du socket TCP");

            // adresse TCP du client
            struct sockaddr_in tcpClientAddr;
            memset(&tcpClientAddr, 0, sizeof(tcpClientAddr));
            tcpClientAddr.sin_family = AF_INET;
            tcpClientAddr.sin_port = htons(PORT);
            tcpClientAddr.sin_addr = clientAddr.sin_addr;

            // Tente de se connecter au client (s'il le souhaite)
            if (connect(tcpFd, (struct sockaddr *) &tcpClientAddr, sizeof(tcpClientAddr)) < 0) {
                perror("Erreur lors de la connexion TCP");
                close(tcpFd);
            } else {
                // Ajouter le nouveau client à la structure
                threadInfo->clients[threadInfo->clientCount].socket = tcpFd;
                memcpy(&(threadInfo->clients[threadInfo->clientCount].addr), &tcpClientAddr,
                       sizeof(tcpClientAddr));
                threadInfo->clientCount++;

                printf("Nouveau client connecté via TCP depuis %s\n", inet_ntoa(clientAddr.sin_addr));
            }
        }

        pthread_mutex_unlock(&threadInfo->mutex);


        //n = send(sockfd, buffer2, 1024, MSG_CONFIRM);
        EXIT_IF_FAIL(n, "Probleme send");
    }
}

void *tcp(void *args) {
    thread_Info *threadInfo = (thread_Info *) args;
    char buffer[BUFFER_SIZE];
    for (;;) {
        for (int i = 0; i < MAX_SERVERS; i++) {

        }
    }
    pthread_exit(NULL);
}


int main(int arvc, char **argv) {
    game_Server gameServer;
    thread_Info threadInfo;
    memset(&gameServer.socket, -1, sizeof(gameServer.socket));
    pthread_mutex_init(&threadInfo.mutex, NULL);

    pthread_t daemon, tcpThread;

    pthread_create(&tcpThread, NULL, tcp, (void *) &threadInfo);
    pthread_create(&daemon, NULL, serveurUdp, (void *) &threadInfo);

    pthread_join(daemon, NULL);
    pthread_join(tcpThread, NULL);

    EXIT_FAILURE;
}