#include "commands.h"
#include "pretty.h"

void cmd_ip(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa ip\nDisplays local IP addresses and subnet masks.\n");
        return;
    }

    struct ifaddrs *addrs, *tmp;
    if (getifaddrs(&addrs) != 0) {
        perror("getifaddrs");
        return;
    }

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

void cmd_mac(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa mac\nDisplays MAC adresses for each interace.\n");
        return;
    }

    struct ifaddrs *addrs, *tmp;
    if (getifaddrs(&addrs) != 0) {
        perror("getifaddrs");
        return;
    }

    printf("Your local MAC address(es):\n\n");
    for(tmp = addrs; tmp != NULL; tmp = tmp->ifa_next) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll *s = (struct sockaddr_ll *)tmp->ifa_addr;
            unsigned char *mac = s->sll_addr; 
            char mac_str[18];
            sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

            printf("  Interface: ");
            print_colored(tmp->ifa_name, "\033[34m");
            printf("    MAC Address: ");
            print_colored(mac_str, "\033[32m");
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
    {"mac", cmd_mac, "Displays MAC adresses for each interace."},
    {NULL, NULL, NULL}
};

void print_help() {
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %s - %s\n", commands[i].name, commands[i].help);
    }
}