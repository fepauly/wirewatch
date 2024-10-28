#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>
#include "pretty.h"
#include "netstuff.h"

void display_help() {
    printf("How to use me:\n");
    printf("  wiwa <COMMAND>\n\n");

    printf("Available Commands:\n");
    printf("  hello      - Displays a welcome message and help information.\n");
    printf("  ip         - Displays local IP adresses on the machine.\n");
    printf("  help       - Displays this help message.\n");
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        print_colored("WOW. You are missing some commands my friend. Try 'wiwa hello' for some help!", "\033[31m");
        return 1;
    } 

    if (strcmp(argv[1], "hello") == 0 ) {
        printf("Hello and welcome to this little WireWatch network scanner my friend!\n\n");
        display_help();
    } else if (strcmp(argv[1], "help") == 0) {
        display_help();
    } else if (strcmp(argv[1], "ip") == 0) {
        show_local_ip(0);
        if (argc > 2 && (strcmp(argv[2], "--details") == 0 || strcmp(argv[2], "-d") == 0)) {
        show_local_ip(1);
}
    } else {
        print_colored("NOPE. That command is not correct my friend. Try 'wiwa help' for some help!", "\033[31m");
    } 
    
}