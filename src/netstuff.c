#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "netstuff.h" 
#include "pretty.h"

void show_local_ip(int details) {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);

    printf("Your local IP address(es):\n\n");
    for(tmp=addrs; tmp != NULL;tmp = tmp->ifa_next) {
        if(tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in *)tmp->ifa_addr)->sin_addr, ip, sizeof(ip));

            printf("  Interface: ");
            print_colored(tmp->ifa_name, "\033[34m");

            printf("    IPv4 Adress: ");
            print_colored(ip, "\033[32m");

        }
    }
    freeifaddrs(addrs);
}