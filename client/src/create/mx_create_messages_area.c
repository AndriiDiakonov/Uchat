#include "../../inc/uchat_client.h"

int mx_create_messages_area(void) {
    if (sockfd == -1){
        if (mx_connect_to_server(&sockfd) < 0)
            return -1;
    }


    t_chat_room_vars.right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_fixed_put(GTK_FIXED(chat_area), t_chat_room_vars.right_container, L_FIELD_WIDTH, 0);

    GtkWidget *room_header_content = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(room_header_content), "room_header");
    gtk_widget_set_size_request(GTK_WIDGET(room_header_content), CUR_WIDTH - L_FIELD_WIDTH, 50);
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.right_container), room_header_content, FALSE, FALSE, 0);
    
    t_chats_list *node = mx_chat_search(&chats_list_head, (int)curr_destination);

    GtkWidget *title = gtk_label_new(node->title);
    gtk_widget_set_name(GTK_WIDGET(title), "room_header_title");
    gtk_box_pack_start(GTK_BOX(room_header_content), title, FALSE, TRUE, 200);
    gtk_widget_set_margin_start(GTK_WIDGET(title), 140);
    gtk_widget_set_halign(GTK_WIDGET(title), GTK_ALIGN_CENTER);

    GtkAdjustment *vadjustment = gtk_adjustment_new(CUR_HEIGHT - 110, 0, CUR_HEIGHT - 110, 0, 0, 0);
    GtkWidget *scroll_area = gtk_scrolled_window_new(NULL, vadjustment);
    gtk_widget_set_size_request(GTK_WIDGET(scroll_area), CUR_WIDTH - L_FIELD_WIDTH, CUR_HEIGHT - 100);
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.right_container), scroll_area, FALSE, FALSE, 0);
    t_chat_room_vars.messages_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(GTK_WIDGET(scroll_area), "chat_background");
    gtk_widget_set_size_request(GTK_WIDGET(t_chat_room_vars.messages_box), CUR_WIDTH - L_FIELD_WIDTH, CUR_HEIGHT - 100);
    gtk_container_add(GTK_CONTAINER(scroll_area), t_chat_room_vars.messages_box);

    char sendBuff[1024];
    sprintf(sendBuff, "LoadRoom\n%u\n%u", t_user.id, curr_destination);

    if(send(sockfd, sendBuff, 1024, 0) == -1){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        gtk_widget_destroy(t_chat_room_vars.right_container);
        t_chat_room_vars.right_container = NULL;
        return -1;
    }

    if(recv(sockfd, &max_msg_id, sizeof(int), 0) == 0){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        gtk_widget_destroy(t_chat_room_vars.right_container);
        t_chat_room_vars.right_container = NULL;
        return -1;
    }

    if (max_msg_id > 0) {
        char recvBuff[1024];
        bzero(recvBuff, 1024);
        for (int i = 0; i < max_msg_id; i++) {
            if(recv(sockfd, recvBuff, 1024, 0) == 0){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return -1;
            }

            int cur_m_id = 0;
            if(recv(sockfd, &cur_m_id, sizeof(int), 0) == 0){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return -1;
            }

            char **recvData = mx_strsplit(recvBuff, '\n');
            int msg_id = mx_atoi(recvData[0]);
            int msg_addresser = mx_atoi(recvData[1]);
            int seconds = mx_atoi(recvData[3]);
            char *text = NULL;
            if (mx_strcmp("(null)", recvData[2]))
                text = mx_strdup(recvData[2]);

            mx_push_back_message(&curr_room_msg_head, 
                text, 
                msg_addresser,
                mx_read_image_message(cur_m_id),
                seconds,
                cur_m_id);

            mx_del_strarr(&recvData);
            bzero(recvBuff, 1024);
        }

        t_message *msg = curr_room_msg_head;
        while (msg != NULL) {
            mx_add_message(t_chat_room_vars.messages_box, msg);
            //max_msg_id = msg->id;
            msg = msg->next;
        }
    }

    gtk_widget_show_all(GTK_WIDGET(t_chat_room_vars.right_container));
    return 0;
}
