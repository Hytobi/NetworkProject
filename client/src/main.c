/** @author: PLOUVIN Patrice, DELCOURT Hana
 * @date: 18/10/2023
 * @brief: Fonction principale du programme
 * @file: src/main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#include "cJSON/cJSON.h"
#include "err.h"
#include "utils.h"

int main(int arvc, char** argv) {
    printf("\033[H\033[2J");
    // Client UDP

    printf("Création du socket...\n");
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    EXIT_IF_FAIL(sockfd, "Probleme creation socket");

    printf("Paramétrage du Broadcast...\n");
    struct sockaddr_in broadcast_addr;
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(42069);
    broadcast_addr.sin_addr.s_addr=INADDR_BROADCAST;

    printf("Autorisation du broadcast...\n");
    int enable=1;
    int socketOpt=setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&enable,sizeof(enable));
    EXIT_IF_FAIL(socketOpt,"Erreur set socket");

    printf("Envoi du message en broadcast...\n");
    char* message = malloc(sizeof(char)*1020);
    strcpy(message,"looking for bomberstudent servers");
    int sendRtr=sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
    EXIT_IF_FAIL(sendRtr,"Send message");


    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024];
    int n = recvfrom(sockfd, buffer, 1020, MSG_WAITALL,(struct sockaddr *) &clientAddr,&clientAddrLen);
    EXIT_IF_FAIL(n, "Msg not recv");

    printf("Message reçu de %s : %s\n", inet_ntoa(clientAddr.sin_addr),buffer);

    return EXIT_SUCCESS;
}