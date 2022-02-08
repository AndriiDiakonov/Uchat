#include "../../inc/uchat_client.h"

void mx_delete_message_from_db(t_message *data) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }
    
    char sendBuff[256];
    bzero(sendBuff, 256);
    sprintf(sendBuff, "DeleteMessage\n%d\n%d\n%d", t_user.id, curr_destination, (int)data->id);
    
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
    
    if(send(sockfd, sendBuff, 256, 0) == -1) {
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }
}
