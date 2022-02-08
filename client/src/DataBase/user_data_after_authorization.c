#include "../../inc/uchat_client.h"

/*
    user_recv_data[0] - Status (SUCCESS or FAIL)
    user_recv_data[1] - ID
    user_recv_data[2] - NAME
    user_recv_data[3] - SURNAME
    user_recv_data[4] - PSEUDONIM
    user_recv_data[5] - DESCRIPTION
*/

int mx_write_user_data_from_bd_after_auth(const char *pseudo, const char* passwd) {
    if (sockfd == -1)
        mx_connect_to_server(&sockfd);
        //return 1;


    char sendBuffer[1025];
    bzero(sendBuffer, 1025);
    sprintf(sendBuffer, "Authorization\n%s\n%s", pseudo, passwd);

    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &error, &len);
    if (retval != 0) {
        fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
        sockfd = -1;
        return 1;
    }
    if (error != 0) {
        fprintf(stderr, "socket error: %s\n", strerror(error));
        sockfd = -1;
        return 1;
    }

    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) == -1) {
        perror("ERROR writing to socket");
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        if(error_revealer == NULL)
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return 1;
    }

    char recvBuffer[6000];
    bzero(recvBuffer, 6000);
    if (recv(sockfd, recvBuffer, 6000, 0) == 0) {
        perror("ERROR reading from socket");
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        if(error_revealer == NULL)
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
         sockfd = -1;
         return 1;
    }

    char **user_recv_data = mx_strsplit(recvBuffer, '\n');
    if (!mx_strcmp(user_recv_data[0], "SUCCESS")) {
        t_user.id = mx_atoi(user_recv_data[1]);

        if (t_user.FirstName != NULL)
            free(t_user.FirstName);
        t_user.FirstName = mx_strdup(user_recv_data[2]);

        if (t_user.SecondName != NULL)
            free(t_user.SecondName);
        t_user.SecondName = mx_strdup(user_recv_data[3]);

        if (t_user.pseudonim != NULL)
            free(t_user.pseudonim);
        t_user.pseudonim = mx_strdup(user_recv_data[4]);

        if (t_user.description != NULL)
            free(t_user.description);
        t_user.description = mx_strdup(user_recv_data[5]);

        if (t_user.avatar != NULL)
            g_object_unref(t_user.avatar);
            
        mx_read_photo_from_bd(t_user.id);
        t_user.avatar = mx_get_pixbuf_with_size("client/img/tmp_avatar.png", 100, 100);
        remove("client/img/tmp_avatar.png");
        //==========================================================================
        mx_del_strarr(&user_recv_data);
    }
    else {
        printf("%s\n", recvBuffer);
        mx_del_strarr(&user_recv_data);
        return 1; 
    }
    return 0;
}

int mx_check_login_reg(const char *pseudo) {
    char sendBuffer[1024];
    bzero(sendBuffer, 1024);
    sprintf(sendBuffer, "FindUser\n%s", pseudo);
    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) < 0) {
         perror("ERROR writing to socket");
    }

    char recvBuffer[1024];
    bzero(recvBuffer, 1024);
    if (recv(sockfd, recvBuffer, 1024, 0) < 0) {
         perror("ERROR reading from socket");
    }
    
    if (!mx_strcmp(recvBuffer, "FAIL"))
        return 0;
    else
        return 1;
}
