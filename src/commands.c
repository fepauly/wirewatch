#include "commands.h"
#include "pretty.h"

/* Default ports to scan */
Port default_ports[] = {
    {20, "FTP (File Transfer Protocol) - Data Transfer"},
    {21, "FTP (File Transfer Protocol) - Control"},
    {22, "SSH (Secure Shell) - Secure Remote Access"},
    {23, "Telnet - Unsecured Remote Access"},
    {25, "SMTP (Simple Mail Transfer Protocol) - Email Sending"},
    {53, "DNS (Domain Name System) - Name Resolution"},
    {67, "DHCP (Dynamic Host Configuration Protocol) - Server"},
    {68, "DHCP (Dynamic Host Configuration Protocol) - Client"},
    {80, "HTTP (Hypertext Transfer Protocol) - Unsecured Web Communication"},
    {110, "POP3 (Post Office Protocol) - Email Retrieval"},
    {143, "IMAP (Internet Message Access Protocol) - Email Retrieval"},
    {443, "HTTPS (HTTP Secure) - Secure Web Communication"},
    {465, "SMTPS (SMTP Secure) - Secure Email Sending"},
    {587, "SMTP (Simple Mail Transfer Protocol) - Email Submission"},
    {3306, "MySQL - Database Access"},
    {5432, "PostgreSQL - Database Access"},
    {6379, "Redis - Database Access"},
    {8080, "HTTP Alternate - Alternative for HTTP"},
    {8443, "HTTPS Alternate - Alternative for HTTPS"}
};

/* Scan Subcommands */

void scan_ip(int argc, char *argv[]) {
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
            print_colored(tmp->ifa_name, BLUE_COLOR);
            printf("    IPv4 Adress: ");
            print_colored(ip, GREEN_COLOR);
            printf("    Subnet Mask: ");
            print_colored(subnet, GREEN_COLOR);
        }
    }
    freeifaddrs(addrs);
}

void scan_port(int argc, char *argv[]){
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa scan port <destination address> [start_port] [end_port]\nScan a destination adress for open default ports, single port or port range.\n");
        return;
    }

    if (argc < 2) {
        print_colored("NOPE NOPE. Usage: wiwa scan port <destination address> [start_port] [end_port]", ERROR_COLOR);
        return;
    }

    const char *destination = argv[1];
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;

    // Resolve destination address
    if (inet_pton(AF_INET, destination, &addr.sin_addr) <= 0) {
        struct hostent *host = gethostbyname(destination);
        if (host == NULL) {
            herror("gethostbyname");
            return;
        }
        addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
    }

    // Determine port range
    int start_port = 0, end_port = 0;
    if (argc == 2) {  // Use default ports
        printf("Scanning default ports for destination %s...\n", destination);
    } else if (argc == 3) {  // Single port
        start_port = end_port = atoi(argv[2]);
        if (start_port <= 0) {
            print_colored("Thats an invalid port my friend. Port must be positive.", ERROR_COLOR);
            return;
        }
        printf("Scanning port %d for destination %s...\n", start_port, destination);
    } else if (argc == 4) {  // Port range
        start_port = atoi(argv[2]);
        end_port = atoi(argv[3]);
        if (start_port <= 0 || end_port <= 0 || start_port > end_port) {
            print_colored("Thats an invalid port range my friend. Ports must be positive and start port must be less than or equal to end port.", ERROR_COLOR);
            return;
        }
        printf("Scanning ports from %d to %d for destination %s...\n", start_port, end_port, destination);
    }

    size_t range_count = (argc == 2) ? sizeof(default_ports) / sizeof(default_ports[0]) : (size_t)(end_port - start_port + 1);
    for (size_t i = 0; i < range_count; i++) {
        int port = (argc == 2) ? default_ports[i].port : start_port + (int)i;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("socket");
            continue;
        }

        addr.sin_port = htons(port);
        struct timeval timeout = {.tv_sec = 0, .tv_usec = 500000};
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            printf("Port %d (%s) is open!\n", port, (argc == 2 ? default_ports[i].name : ""));
        } else {
            if (start_port == end_port) {
            printf("Port %d is closed!\n", port);
            }
        }

        close(sockfd);
    }
}

/* Commands */

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
            print_colored(tmp->ifa_name, BLUE_COLOR);
            printf("    MAC Address: ");
            print_colored(mac_str, GREEN_COLOR);
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
            print_colored(gateway_str, GREEN_COLOR);
            
            fclose(fp);
            return;
        }
    }

    print_colored("No default gateway found.", ERROR_COLOR);
    fclose(fp);
}

void cmd_ping(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa ping <destination adress>\nPings a destination adress and returns a summary.\n");
        return;
    }
    if (argc != 2) {
        print_colored("NOPE. You need to add a destination adress my friend: wiwa ping <destination adress>!", ERROR_COLOR);
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

    if (inet_pton(AF_INET, destination, &addr.sin_addr) <= 0) {
        struct hostent *host = gethostbyname(destination);
        if (host == NULL) {
            herror("gethostbyname");
            return;
        }
        addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
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

void cmd_scan(int argc, char *argv[]) {
    if (argc < 2) {
        print_colored("NOPE. Usage: wiwa scan <subcommand>", ERROR_COLOR);
        return;
    }

    if (strcmp(argv[1], "help") == 0) {
        print_scan_help();
        return;
    }

    for (int i = 0; scan_commands[i].name != NULL; i++) {
        if (strcmp(argv[1], scan_commands[i].name) == 0) {
            scan_commands[i].function(argc - 1, argv + 1);
            return;
        }
    }

    print_colored("NOPE. I don't know that command my friend.\n", ERROR_COLOR);
    print_scan_help();
    return;
}


void cmd_hello(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa hello\nPrints a little welcome message.\n");
        return;
    }
    print_colored("Hello and welcome to this little WireWatch network scanner my friend!\n\n", GREEN_COLOR);
}

Command commands[] = {
    {"hello", cmd_hello, "Prints a little welcome message."},
    {"mac", cmd_mac, "Displays MAC adresses for each interface."},
    {"gateway", cmd_gateway, "Displays the default gateway adress."},
    {"ping", cmd_ping, "Pings a destination adress and returns a summary."},
    {"scan", cmd_scan, "Used to scan some cool things, e.g. open ports for a destination adress."},
};

Command scan_commands[] = {
    {"port", scan_port, "Scan open ports for a destination adress."},
    {"ip", scan_ip, "Show local IP adresses and subnet masks."},
    {NULL, NULL, NULL}
};


void print_help() {
    printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %s - %s\n", commands[i].name, commands[i].help);
    }
}

void print_scan_help() {
    printf("Available subcommands:\n");
    for (int i = 0; scan_commands[i].name != NULL; i++) {
        printf("  %s - %s\n", scan_commands[i].name, scan_commands[i].help);
    }
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