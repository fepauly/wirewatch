#include "commands.h"
#include "pretty.h"

void cmd_ip(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa ip\nDisplays local IP addresses and subnet masks.\n");
        return;
    }

    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);

    printf("Your local IP address(es):\n\n");
    for(tmp=addrs; tmp != NULL;tmp = tmp->ifa_next) {
        if(tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            char subnet[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in *)tmp->ifa_addr)->sin_addr, ip, sizeof(ip));
            inet_ntop(AF_INET, &((struct sockaddr_in *)tmp->ifa_netmask)->sin_addr, subnet, sizeof(subnet));

            // Print ip information
            printf("  Interface: ");
            print_colored(tmp->ifa_name, "\033[34m");
            printf("    IPv4 Adress: ");
            print_colored(ip, "\033[32m");
            printf("    Subnet Mask: ");
            print_colored(subnet, "\033[32m");
        }
    }
    freeifaddrs(addrs);
}

void cmd_hello(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa hello\nPrints a little welcome message.\n");
        return;
    }
    printf("Hello and welcome to this little WireWatch network scanner my friend!\n\n");
}

Command commands[] = {
    {"ip", cmd_ip, "Show local IP adresses and subnet masks."},
    {"hello", cmd_hello, "Prints a little welcome message."},
    {NULL, NULL, NULL}
};

void print_help() {
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %s - %s\n", commands[i].name, commands[i].help);
    }
}