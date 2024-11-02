#include "pretty.h"

// Function for colored print
void print_colored(const char *color, const char *format, ...) {
    va_list args;             
    va_start(args, format);

    printf("%s", color); // set color
    vprintf(format, args);
    printf("\033[0m\n"); // reset color

    va_end(args);     
}