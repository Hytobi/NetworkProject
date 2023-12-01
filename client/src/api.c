
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "utils.h"
#include "rage.h"
#include "api.h"

void post(char* ip, char* data) {
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
    addr.sin_port = htons(PORT);
    // on envoie a l'addresse de
    addr.sin_addr.s_addr = inet_addr(ip);

    // bind sert a lier le socket a une adresse
    EXIT_IF_FAIL(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)), "Probleme bind");
    // fcntl sert a configurer le socket
    EXIT_IF_FAIL(fcntl(sockfd, F_SETFL, MSG_WAITALL), "Probleme fcntl");

    // on envoie le message
    int n;
    n = sendto(sockfd, data, strlen(data), MSG_CONFIRM, (struct sockaddr*)&addr, sizeof(addr));
    EXIT_IF_FAIL(n, "Probleme send");

}

void get(char* ip, char* path) {
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
    addr.sin_port = htons(PORT);
    // on envoie a l'addresse de
    addr.sin_addr.s_addr = inet_addr(ip);

    EXIT_IF_FAIL(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)), "Probleme bind");
    EXIT_IF_FAIL(fcntl(sockfd, F_SETFL, MSG_WAITALL), "Probleme fcntl");

    int n;
    n = sendto(sockfd, path, 1020, MSG_CONFIRM, (struct sockaddr*)&addr, sizeof(addr));
    EXIT_IF_FAIL(n, "Probleme send");

}