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