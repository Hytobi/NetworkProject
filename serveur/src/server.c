//
// Created by hana on 01/12/23.
//

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

#include "server.h"
#include "err.h"

int initServ(int port){
    int socketFd; // Le file descriptor du socket
    if ((socketFd=socket(AF_INET,SOCK_DGRAM,0)) == ERR){
        PERROR("Erreur création du socket",5);
    }
}