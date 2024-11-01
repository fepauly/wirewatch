#ifndef PRETTY_H
#define PRETTY_H

#include <stdio.h>

#define ERROR_COLOR  "\033[31m"
#define BLUE_COLOR   "\033[34m"
#define GREEN_COLOR  "\033[32m"

void print_colored(const char *message, const char *color);

#endif // PRETTY_H