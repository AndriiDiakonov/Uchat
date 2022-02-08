#include "../../inc/uchat_client.h"

static void *check_last_room(void *data);

void mx_configure_chats_list(void) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }
    chats_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_fixed_put(GTK_FIXED(chat_area), chats_list, 0, 105);
    gtk_widget_set_name(GTK_WIDGET(chats_list), "chats_list");

    char sendBuffer[1024];
    bzero(sendBuffer, 1024);
    sprintf(sendBuffer, "GetUsersArr\n%d", t_user.id);

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

    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) == -1) {
        perror("ERROR writing to socket");
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }
    
    int rooms_uids_len = 0;
    
    if(recv(sockfd, &rooms_uids_len, sizeof(int), 0) == 0){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    unsigned int *rooms_uids = (unsigned int *)malloc(rooms_uids_len);
    for (int i = 0; i < rooms_uids_len; i++)
        if(recv(sockfd, &rooms_uids[i], sizeof(unsigned int), 0) == 0){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return;
        }
    
    for (int i = 0; i < rooms_uids_len; i++)
        gtk_box_pack_start(GTK_BOX(chats_list), 
            mx_create_room(rooms_uids[i], L_FIELD_WIDTH, room_click), FALSE, FALSE, 0);
    
    free(rooms_uids);

    pthread_create(&check_last_room_id, NULL, check_last_room, NULL);
}

static void *check_last_room(void *data) {
    mx_connect_to_server(&sock_for_rooms);
    while (true) {
        if (sock_for_rooms == -1) {
            mx_connect_to_server(&sock_for_rooms);
            continue;
        }
        t_chats_list *node = chats_list_head;
        int last_uid = 0;

        GList *children = gtk_container_get_children(GTK_CONTAINER(chats_list));
        while (true) {
            if (node->room == g_list_nth_data(children, 0)) {
                last_uid = node->uid;
                break;
            }
            else
                node = node->next;
        }
        g_list_free(children);

        
        char sendBuff[256];
        bzero(sendBuff, 256);
        sprintf(sendBuff, "CheckLastRoom\n%d\n%d", t_user.id, (int)last_uid);

        if(send(sock_for_rooms, sendBuff, 256, 0) == -1){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sock_for_rooms = -1;
            return NULL;
        }
        

        int serv_last_uid = 0;

        if(recv(sock_for_rooms, &serv_last_uid, sizeof(int), 0) == 0){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sock_for_rooms = -1;
            return NULL;
        }

        if (serv_last_uid != last_uid) {
            node = chats_list_head;
            while (node != NULL) {
                if (node->uid == serv_last_uid) {
                    gtk_box_reorder_child(GTK_BOX(chats_list), node->room, 0);
                    gtk_widget_set_state_flags(GTK_WIDGET(node->room), GTK_STATE_FLAG_LINK, FALSE);
                    break;
                }
                else
                    node = node->next;
            }
            if (node == NULL) {
                gtk_box_pack_start(GTK_BOX(chats_list), 
                    mx_create_room((unsigned int)serv_last_uid, L_FIELD_WIDTH, room_click), FALSE, FALSE, 0);
                node = chats_list_head;
                while (node->uid != serv_last_uid)
                    node = node->next;
                gtk_box_reorder_child(GTK_BOX(chats_list), node->room, 0);
                gtk_widget_set_state_flags(GTK_WIDGET(node->room), GTK_STATE_FLAG_LINK, TRUE);
                gtk_widget_show_all(GTK_WIDGET(node->room));
            }
        }
        usleep(500000);
    }

    return NULL;
}
