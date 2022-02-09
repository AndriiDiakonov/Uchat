#include "../inc/libmx.h"

void mx_check_argv(int argc, char **argv) {
    char *errmsg;
    if (argc != 2) {
        errmsg = "usage: ./server [port]\n";
        write(2, errmsg, (int)mx_strlen(errmsg));
    }
}
