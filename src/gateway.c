#include "gateway.h"

int cmd_gateway(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa gateway\nDisplays the default gateway adress.\n");
        return 0;
    }

    FILE *fp = fopen("/proc/net/route", "r");
    if(fp == NULL) {
        perror("Error opening '/proc/net/route'");
        return 1;
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
            
            printf("Default Gateway (via %s): ", iface);
            print_colored(GREEN_COLOR, inet_ntoa(gw_addr));
            
            fclose(fp);
            return 0;
        }
    }

    print_colored(ERROR_COLOR, "No default gateway found." );
    fclose(fp);
    return 0;
}