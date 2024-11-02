#include "scan.h"

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

/* Subcommands */

int cmd_scan(int argc, char *argv[]) {
    if (argc < 2) {
        print_colored("NOPE. Usage: wiwa scan <subcommand>", ERROR_COLOR);
        return 1;
    }

    if (strcmp(argv[1], "help") == 0) {
        print_scan_help();
        return 0;
    }

    for (int i = 0; scan_commands[i].name != NULL; i++) {
        if (strcmp(argv[1], scan_commands[i].name) == 0) {
            scan_commands[i].function(argc - 1, argv + 1);
            return 0;
        }
    }

    print_colored("NOPE. I don't know that command my friend.\n", ERROR_COLOR);
    print_scan_help();
    return 0;
}

/* Scan Subcommands */

int scan_ip(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa ip\nDisplays local IP addresses and subnet masks.\n");
        return 0;
    }

    struct ifaddrs *addrs, *tmp;
    if (getifaddrs(&addrs) != 0) {
        perror("getifaddrs");
        return 1;
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
    return 0;
}

int scan_port(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa scan port <destination address> [start_port] [end_port]\nScan a destination adress for open default ports, single port or port range.\n");
        return 0;
    }

    if (argc < 2) {
        print_colored("NOPE. Usage: wiwa scan port <destination address> [start_port] [end_port]", ERROR_COLOR);
        return 1;
    }

    const char *destination = argv[1];
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;

    // Resolve destination address
    if (inet_pton(AF_INET, destination, &addr.sin_addr) <= 0) {
        struct hostent *host = gethostbyname(destination);
        if (host == NULL) {
            perror("gethostbyname");
            return 1;
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
            return 1;
        }
        printf("Scanning port %d for destination %s...\n", start_port, destination);
    } else if (argc == 4) {  // Port range
        start_port = atoi(argv[2]);
        end_port = atoi(argv[3]);
        if (start_port <= 0 || end_port <= 0 || start_port > end_port) {
            print_colored("Thats an invalid port range my friend. Ports must be positive and start port must be less than or equal to end port.", ERROR_COLOR);
            return 1;
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
    return 0;
}