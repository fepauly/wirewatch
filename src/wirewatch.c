#include "commands.h"
#include "pretty.h"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        print_colored("WOW. You are missing some commands my friend. Try 'wiwa help' for some help!", "\033[31m");
        return 1;
    } 

    if (strcmp(argv[1], "help") == 0) {
        print_help();
        return 0;
    }

    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[1], commands[i].name) == 0) {
            commands[i].function(argc - 1, argv + 1);
            return 0;
        }
    }

    print_colored("NOPE. I don't know that command my friend", "\033[31m");
    print_help();
    return 1;
    
}