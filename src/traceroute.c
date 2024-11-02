#include "traceroute.h"

int cmd_traceroute(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: (sudo) wiwa traceroute <destination adress> \nTrace the path packets take to a destination with ICMP echo requests.\n");
        return 0;
    }

    if (argc != 2) {
        print_colored(ERROR_COLOR, "NOPE. You need to add a destination adress my friend: wiwa traceroute <destination adress>!");
        return 1;
    }

    const char *destination = argv[1];

    int sockfd;
    struct sockaddr_in addr; // destination adress
    struct icmphdr packet;
    socklen_t addr_len = sizeof(addr);
    char recv_buffer[64];
    struct timeval timeout = {1, 0};
    int max_hops = 30;

    // Create raw socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0 ) {
        perror("socket");
        return 1;
    }

    // Set destination adress
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    if (inet_pton(AF_INET, destination, &addr.sin_addr) <= 0) {
        struct hostent *host = gethostbyname(destination);
        if (host == NULL) {
            perror("gethostbyname");
            return 1;
        }
        addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
    }

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    char destination_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, destination_ip, sizeof(destination_ip));

    for (int ttl = 1; ttl <= max_hops; ttl++) {
        setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        // Create ICMP packet
        memset(&packet, 0, sizeof(packet));
        packet.type = ICMP_ECHO;
        packet.un.echo.id = getpid();
        packet.un.echo.sequence = ttl;
        packet.checksum = calc_checksum(&packet, sizeof(packet));

        if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *) &addr, addr_len) <= 0) {
            perror("sendto");
            return 1;
        }

        struct sockaddr_in reply_addr;
        socklen_t reply_len = sizeof(reply_addr);
        if (recvfrom(sockfd, &recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&reply_addr, &reply_len) < 0) {
            print_colored(ERROR_COLOR, "Error at stop %d", ttl);
        } else {
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &reply_addr.sin_addr, ip_str, sizeof(ip_str));
            printf("Stop %d: ", ttl);
            print_colored(BLUE_COLOR, ip_str);

            if (strcmp(ip_str, destination_ip) == 0) {
                print_colored(GREEN_COLOR, "You reached the destination my friend.");
                break;
            }
        }
    }
    close(sockfd);
    return 0;
}