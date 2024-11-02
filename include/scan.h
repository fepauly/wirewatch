#ifndef SCAN_H
#define SCAN_H

#include "pretty.h"
#include "commands.h"
#include "netstuff.h"
#include "helpers.h"

int cmd_scan(int argc, char *argv[]);
int scan_ip(int argc, char *argv[]);
int scan_port(int argc, char *argv[]);

extern Port default_ports[];

#endif