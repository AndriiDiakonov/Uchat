#include "../inc/server.h"

void mx_check_port(int port) {
    if (port <= 0) {
    	char *errmsg = "Invalid port number\n";
        write(2, errmsg, (int)strlen(errmsg));
        exit(1);
    }
}
