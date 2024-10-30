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

void cmd_gateway(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa gateway\nDisplays the default gateway adress.\n");
        return;
    }

    FILE *fp = fopen("/proc/net/route", "r");
    if(fp == NULL) {
        perror("Error opening '/proc/net/route'");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp);  // Skip header line

    while (fgets(line, sizeof(line), fp) != NULL) {
        char iface[16];
        unsigned long destination, gateway;

        // Read interface name, destination and gateway from line -> Returns 3 if all 3 variables are filled
        if(sscanf(line, "%15s %lx %lx", iface, &destination, &gateway) != 3) { 
            continue;
        }

        // Check if this is the default route (== 0)
        if (destination == 0) {
            struct in_addr gw_addr;
            gw_addr.s_addr = gateway;
            
            char gateway_str[INET_ADDRSTRLEN];
            sprintf(gateway_str, "%s", inet_ntoa(gw_addr));
            printf("Default Gateway (via %s): ", iface);
            print_colored(gateway_str, "\033[32m");
            
            fclose(fp);
            return;
        }
    }

    print_colored("No default gateway found.", "\033[31m");
    fclose(fp);
}

void cmd_ping(int argc, char *argv[]) {
    if (argc != 2) {
        print_colored("NOPE. You need to add a destination adress my friend: wiwa ping <destination adress>!", "\033[31m");
        return;
    }

    const char *destination = argv[1];

    int sockfd;
    struct sockaddr_in addr; // destination adress
    struct icmphdr packet;
    struct timespec start, end;
    socklen_t addr_len = sizeof(addr);

    // Create raw socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0 ) {
        perror("socket");
        return;
    }

    // Set destination adress
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    if (is_valid_ip(destination)) {
        addr.sin_addr.s_addr = inet_addr(destination);
    } else {
        struct hostent *host = gethostbyname(destination);
        if (host == NULL) {
            herror("gethostbyname");
            return;
        }

        addr.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
    }

    // Create ICMP packet
    memset(&packet, 0, sizeof(packet));
    packet.type = ICMP_ECHO;
    packet.un.echo.id = getpid();
    packet.un.echo.sequence = 1;
    packet.checksum = checksum(&packet, sizeof(packet));

    // Send the packet
    clock_gettime(CLOCK_MONOTONIC, &start); // Start time
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *) &addr, addr_len) <= 0) {
        herror("sendto");
        exit(EXIT_FAILURE);
    }

    // Wait for answer
    char buf[1024];
    if (recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len) <= 0) {
        herror("revcfrom");
        exit(EXIT_FAILURE);
    } else {
        clock_gettime(CLOCK_MONOTONIC, &end); // Get end time
        double time_taken = (end.tv_sec - start.tv_sec) * 1000.0; // time in milliseconds
        time_taken += (end.tv_nsec - start.tv_nsec) / 1000000.0;// add nanoseconds
        printf("Received from %s: seq=%d time=%.2f ms\n", inet_ntoa(addr.sin_addr), packet.un.echo.sequence, time_taken);
    }
    close(sockfd);
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
    {"mac", cmd_mac, "Displays MAC adresses for each interface."},
    {"gateway", cmd_gateway, "Displays the default gateway adress."},
    {"ping", cmd_ping, "Pings a destination adress and returns a summary."},
    {NULL, NULL, NULL}
};

void print_help() {
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %s - %s\n", commands[i].name, commands[i].help);
    }
}

int is_valid_ip(const char *ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &sa.sin_addr) == 1;
}

// Calculate the checksum (RFC 1071)
int checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    int result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}