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

#include "err.h"
#include "cJSON/cJSON.h"



int main(int arvc, char** argv) {
    printf("\033[H\033[2J");
    // Création du socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    EXIT_IF_FAIL(sockfd, "Probleme creation socket");

    // Configurer le socket pour qu'il puisse être réutilisé
    EXIT_IF_FAIL(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)), "problem setsockopt");

    //  il faut attacher le socket à un port Internet et une adresse IP
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi("42069"));
    //addr.sin_port = htons(atoi("12345"));
    addr.sin_addr.s_addr = INADDR_ANY;

    EXIT_IF_FAIL(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)), "Probleme bind");
    EXIT_IF_FAIL(fcntl(sockfd, F_SETFL, MSG_WAITALL), "Probleme fcntl");

    printf("Serveur up\n");

    char buffer[1020];
    int n=0;
    for(;;){
        buffer[n]='\0';
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        n = recvfrom(sockfd, buffer, 1020, MSG_WAITALL,(struct sockaddr *) &clientAddr,&clientAddrLen);
        EXIT_IF_FAIL(n, "Msg not recv");

        printf("Message reçu de %s : %s", inet_ntoa(clientAddr.sin_addr),buffer);

        printf("%s\n",buffer);

        char buffer2[1024];
        //sprintf(buffer2,"yes");
        n = sendto(sockfd, buffer2, 1024, MSG_CONFIRM, (struct sockaddr*)&clientAddr, clientAddrLen);

        //n = send(sockfd, buffer2, 1024, MSG_CONFIRM);
        EXIT_IF_FAIL(n, "Probleme send");
    }

    EXIT_FAILURE;
}