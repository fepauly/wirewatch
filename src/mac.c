#include "mac.h"

int cmd_mac(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa mac\nDisplays MAC adresses for each interace.\n");
        return 0;
    }

    struct ifaddrs *addrs, *tmp;
    if (getifaddrs(&addrs) != 0) {
        perror("getifaddrs");
        return 1;
    }

    printf("Your local MAC address(es):\n\n");
    for(tmp = addrs; tmp != NULL; tmp = tmp->ifa_next) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll *s = (struct sockaddr_ll *)tmp->ifa_addr;
            unsigned char *mac = s->sll_addr; 
            char mac_str[18];
            sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

            printf("  Interface: ");
            print_colored(BLUE_COLOR, tmp->ifa_name);
            printf("    MAC Address: ");
            print_colored(GREEN_COLOR, mac_str);
        }
    }
    freeifaddrs(addrs); 
    return 0;
}