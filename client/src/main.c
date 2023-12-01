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
#include <fcntl.h>
#include <arpa/inet.h>

#include "cJSON/cJSON.h"
#include "err.h"

int main(int arvc, char** argv) {
    printf("\033[H\033[2J");
    // Client UDP
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
    // on envoie a l'addresse de 127.0.0.1
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    EXIT_IF_FAIL(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)), "Probleme bind");
    EXIT_IF_FAIL(fcntl(sockfd, F_SETFL, MSG_WAITALL), "Probleme fcntl");

    printf("Client up\n");

    char buffer[1020];
    int n;
    // lit le terminal
    fgets(buffer, 1020, stdin);

    n = sendto(sockfd, buffer, 1020, MSG_CONFIRM, (struct sockaddr*)&addr, sizeof(addr));
    //n = send(sockfd, buffer, 1020, MSG_CONFIRM);
    EXIT_IF_FAIL(n, "Probleme send");


    return EXIT_SUCCESS;
}