#include "getipv4.h"

int cmd_getipv4(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa getipv4 <destination adress> \nGets the IPv4 adress from a hostname.\n");
        return 0;
    }

    if (argc != 2) {
        print_colored(ERROR_COLOR, "NOPE. Please give me a destination adress my friend: wiwa getipv4 <destination adress>");   
        return 1;
    }
    const char *destination = argv[1];
    struct sockaddr_in addr; // destination adress

    if(getipv4(destination, &addr) != 0) {
        char error_message[100];
        sprintf(error_message, "WOW. There was an error while converting the host name %s to an IPv4 adress!", destination);
        return 1;
    }

    char ip_str[INET_ADDRSTRLEN];

    if(inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str)) == NULL) {
        perror("inet_ntop");
        return 1;
    }

    print_colored(GREEN_COLOR, "IPv4 Address for host %s is %s", destination, ip_str);
    return 0;
}