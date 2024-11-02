#include "ping.h"

int cmd_ping(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa ping <destination adress>\nPings a destination adress and returns a summary.\n");
        return 0;
    }
    if (argc != 2) {
        print_colored(ERROR_COLOR, "NOPE. You need to add a destination adress my friend: wiwa ping <destination adress>!");
        return 1;
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
        return 1;
    }

    // Set destination adress
    memset(&addr, 0, sizeof(addr));
    getipv4(destination, &addr);

    // Create ICMP packet
    memset(&packet, 0, sizeof(packet));
    packet.type = ICMP_ECHO;
    packet.un.echo.id = getpid();
    packet.un.echo.sequence = 1;
    packet.checksum = calc_checksum(&packet, sizeof(packet));

    // Send the packet
    clock_gettime(CLOCK_MONOTONIC, &start); // Start time
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *) &addr, addr_len) <= 0) {
        perror("sendto");
        return 1;
    }

    // Wait for answer
    char buf[1024];
    if (recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len) <= 0) {
        perror("revcfrom");
        return 1;
    } else {
        clock_gettime(CLOCK_MONOTONIC, &end); // Get end time
        double time_taken = (end.tv_sec - start.tv_sec) * 1000.0; // time in milliseconds
        time_taken += (end.tv_nsec - start.tv_nsec) / 1000000.0;// add nanoseconds
        printf("Received from %s: seq=%d time=%.2f ms\n", inet_ntoa(addr.sin_addr), packet.un.echo.sequence, time_taken);
    }
    close(sockfd);
    return 0;
}