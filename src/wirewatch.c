#include "wirewatch.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_colored("WOW. You are missing some commands my friend. Try 'wiwa help' for some help!", ERROR_COLOR);
        return 1;
    }

    // Display help if requested
    if (strcmp(argv[1], "help") == 0) {
        print_help();
        return 0;
    }

    // Match command to function
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[1], commands[i].name) == 0) {
            return commands[i].function(argc - 1, argv + 1);
        }
    }

    print_colored("NOPE. I don't know that command my friend.\n", ERROR_COLOR);
    print_help();
    return 1;
}