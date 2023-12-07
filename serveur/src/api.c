//
// Created by hana on 04/12/23.
//

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "api.h"
#include "struct.h"
#include "err.h"

void *clientCommunication(void *args) {
    client *cl = (client *) args;
    char buffer[BUFFER_SIZE];
    int n = 0;

    for (;;) {
        socklen_t clientAddrLen = sizeof(cl->addr);
        //n = recvfrom(cl->socket, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &cl->addr, &clientAddrLen);
        n= read(cl->socket,buffer,BUFFER_SIZE);
        if (n == ERR) {
            perror("Erreur reception du message");
            continue;
        }
        buffer[n] = '\0';

        printf("%s : %s", inet_ntoa(cl->addr.sin_addr), buffer);

        char buffer2[1024];
        sprintf(buffer2, "%s", notifClientServeurUp);
        //sprintf(buffer2,"yes");
        n = sendto(cl->socket, buffer2, BUFFER_SIZE, MSG_CONFIRM, (struct sockaddr *) &cl->addr, clientAddrLen);
        if (n==ERR){
            perror("Erreur envoie du message");
            continue;
        }
    }
    close(cl->socket);
}