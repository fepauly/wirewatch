#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/if_ether.h> 
#include <netpacket/packet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
    const char *name;
    void (*function)(int, char**);
    const char *help;
} Command;

typedef struct {
    const int port;
    const char* name;
} Port;

extern Command commands[];
extern Command scan_commands[];
extern Port default_ports[];
void print_help();
void print_scan_help();
int is_valid_ip(const char *ip);
int checksum(void *b, int len);


#endif // COMMANDS_H