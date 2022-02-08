#include "../../inc/uchat_client.h"

void mx_edit_name(char* name, char* surname, char* pseudo, char *description, int id) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }
    
    char sendBuffer[1024];
    bzero(sendBuffer, 1024);
    sprintf(sendBuffer, "UpdateUserData\n%s\n%s\n%s\n%s\n%d", 
            name, surname, pseudo, description, id);

    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &error, &len);
    if (retval != 0) {
        fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
        sockfd = -1;
        return;
    }
    if (error != 0) {
        fprintf(stderr, "socket error: %s\n", strerror(error));
        sockfd = -1;
         return;
    }
    
    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) < 0) {
        perror("ERROR writing to socket");
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }
}
