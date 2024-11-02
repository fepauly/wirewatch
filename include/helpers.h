#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include "commands.h"
#include "netstuff.h"

void print_help();
void print_scan_help();
int calc_checksum(void *b, int len);
int getipv4(const char* destination, struct sockaddr_in *addr);

#endif