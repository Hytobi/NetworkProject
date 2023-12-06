//
// Created by hana on 06/12/23.
//

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <pthread.h>
#include "netinet/in.h"

#define BUFFER_SIZE 1024
#define MAX_SERVERS 8
#define MAX_CLIENTS 2048
#define PORT 42069

typedef struct client {
    int socket;
    struct sockaddr_in addr;
    int connecter; /** 0: pas connecte, 1: connecte, 2: en cours de connection */
} client;

typedef struct thread_Info {
    client clients[MAX_CLIENTS];
    pthread_mutex_t mutex;
} thread_Info;

#endif //PROJECT_STRUCT_H
