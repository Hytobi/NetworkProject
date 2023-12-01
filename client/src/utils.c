

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
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "rage.h"

void getIPAddressAndMask(char *ipAddress, char *networkMask) {
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;
    getifaddrs(&ifap); // Obtient les adresses des interfaces
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            strcpy(ipAddress, addr);
            sa = (struct sockaddr_in *) ifa->ifa_netmask;
            addr = inet_ntoa(sa->sin_addr);
            strcpy(networkMask, addr);
            break; // Rompt après avoir trouvé la première interface IPv4
        }
    }
    freeifaddrs(ifap);
}

char* getBroadcastAddress() {
    char *broadcastAddress = malloc(16 * sizeof(char));
    char *ipAddress = malloc(16 * sizeof(char));
    char *networkMask = malloc(16 * sizeof(char));
    getIPAddressAndMask(ipAddress, networkMask);

    char *ipAddressToken = strtok(ipAddress, ".");
    char *networkMaskToken = strtok(networkMask, ".");
    char *broadcastAddressToken = strtok(broadcastAddress, ".");

    while (ipAddressToken != NULL) {
        int ipAddressTokenInt = atoi(ipAddressToken);
        int networkMaskTokenInt = atoi(networkMaskToken);
        int broadcastAddressTokenInt = ipAddressTokenInt | ~networkMaskTokenInt;
        sprintf(broadcastAddressToken, "%d", broadcastAddressTokenInt);

        ipAddressToken = strtok(NULL, ".");
        networkMaskToken = strtok(NULL, ".");
        broadcastAddressToken = strtok(NULL, ".");
    }

    free(ipAddress);
    free(networkMask);

    return broadcastAddress;
}