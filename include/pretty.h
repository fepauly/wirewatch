#ifndef PRETTY_H
#define PRETTY_H

#include <stdio.h>
#include <stdarg.h>

#define ERROR_COLOR  "\033[31m"
#define BLUE_COLOR   "\033[34m"
#define GREEN_COLOR  "\033[32m"

void print_colored(const char *color, const char *format, ...) ;

#endif // PRETTY_H