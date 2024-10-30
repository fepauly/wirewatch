#include "pretty.h"

// Function for colored print
void print_colored(const char *message, const char *color) {
    printf("%s%s\033[0m\n", color, message);
} 