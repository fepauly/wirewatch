#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

typedef struct {
    const char *name;
    void (*function)(int, char**);
    const char *help;
} Command;

extern Command commands[];
void print_help();

#endif // COMMANDS_H