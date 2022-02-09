#include "../inc/server.h"

static void deamon_successfull_output(pid_t pid) {
    write(1, "uchat_server started successfully\n", 34);
    write(1, "uchat_server pid: ", 18);
    mx_printint(pid);
    write(1, "\nto STOP uchat_server: kill <uchat_server pid>\n", 47);
}

void mx_deamon(void) {
    pid_t pid;
    pid = fork();
    if(pid < 0) {
        write(2, "error: deamon launch failed\n", 28);
        write(2, "forking error\n", 15);
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    else {
        deamon_successfull_output(getpid());
        umask(0);   
        close(STDIN_FILENO);   
        close(STDOUT_FILENO);  
        close(STDERR_FILENO);   
        setsid();   
    }
}
