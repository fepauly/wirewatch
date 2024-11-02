#include "helpers.h"

void print_help() {
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %s - %s\n", commands[i].name, commands[i].help);
    }
}

void print_scan_help() {
    printf("Available subcommands:\n");
    for (int i = 0; scan_commands[i].name != NULL; i++) {
        printf("  %s - %s\n", scan_commands[i].name, scan_commands[i].help);
    }
}

// Calculate the checksum (RFC 1071)
int calc_checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    int result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int getipv4(const char* destination, struct sockaddr_in *addr) {
    addr->sin_family = AF_INET;
    if(inet_pton(AF_INET, destination, &addr->sin_addr) == 1) {
        return 0; // Successfull conversion of IP address
    }

    // conversion failed, try converting host name
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // only ipv4

    int status = getaddrinfo(destination, NULL, &hints, &res);
    if (status != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    *addr = *(struct sockaddr_in *)res->ai_addr;
    freeaddrinfo(res);
    return 0;
}