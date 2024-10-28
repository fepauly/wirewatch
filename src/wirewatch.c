#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>

// Function for colored print
void print_colored(const char *message, const char *color) {
    printf("%s%s\033[0m\n", color, message);
} 

void show_local_ip() {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);

    printf("Your local IP address:\n");
    for(tmp=addrs; tmp != NULL;tmp = tmp->ifa_next) {
        if(tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in *)tmp->ifa_addr)->sin_addr, ip, sizeof(ip));
            print_colored(ip, "\033[32m");
        }
    }
    freeifaddrs(addrs);
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        print_colored("WOW. You are missing some commands my friend. Try 'wiwa hello' for some help!", "\033[31m");
        return 1;
    } 

    if (strcmp(argv[1], "hello") ==0 ) {
        printf("Hello and welcome to this little WireWatch network scanner my friend!\n");
        show_local_ip();
    } else {
        print_colored("NOPE. That command is not correct my friend. Try 'wiwa hello' for some help!", "\033[31m");
    } 
    
}