#include "commands.h"
#include "pretty.h"
#include "traceroute.h"
#include "scan.h"
#include "ping.h"
#include "mac.h"
#include "gateway.h"
#include "getipv4.h"

int cmd_hello(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "help") == 0) {
        printf("Usage: wiwa hello\nPrints a little welcome message.\n");
        return 0;
    }
    print_colored(GREEN_COLOR,"Hello and welcome to this little WireWatch network scanner my friend!\n\n");
    return 0;
}

Command commands[] = {
    {"hello", cmd_hello, "Prints a little welcome message."},
    {"mac", cmd_mac, "Displays MAC adresses for each interface."},
    {"gateway", cmd_gateway, "Displays the default gateway adress."},
    {"ping", cmd_ping, "Pings a destination adress and returns a summary."},
    {"scan", cmd_scan, "Used to scan some cool things, e.g. open ports for a destination adress."},
    {"traceroute", cmd_traceroute, "Trace the path packets take to a destination with ICMP echo requests."},
    {"getipv4", cmd_getipv4, "Gets the IPv4 adress from a hostname."},
    {NULL, NULL, NULL}
};

Command scan_commands[] = {
    {"port", scan_port, "Scan open ports for a destination adress."},
    {"ip", scan_ip, "Show local IP adresses and subnet masks."},
    {NULL, NULL, NULL}
};