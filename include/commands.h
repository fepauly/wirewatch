#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *name;
    int (*function)(int, char**);
    const char *help;
} Command;

typedef struct {
    const int port;
    const char* name;
} Port;

extern Command commands[];
extern Command scan_commands[];

#endif // COMMANDS_H