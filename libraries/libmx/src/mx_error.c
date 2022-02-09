#include "../inc/libmx.h"

void mx_error(char *msg, int status) {
    if (status < 0) {
        perror(msg);
        //exit(1);
    }
}
