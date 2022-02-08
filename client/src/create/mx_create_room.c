#include "../../inc/uchat_client.h"

GtkWidget *mx_create_room(unsigned int uid, gint width, 
    void (*func)(GtkWidget *, GdkEventButton *, gpointer)) {
        
    GtkWidget *eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(eventbox), "eventbox_room");
    g_signal_connect(G_OBJECT(eventbox), "button_press_event",
        G_CALLBACK(*func), (gpointer)(uintptr_t)uid);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_margin_top(GTK_WIDGET(box), 5);
    gtk_widget_set_margin_bottom(GTK_WIDGET(box), 5);
    gtk_widget_set_size_request(GTK_WIDGET(box), width, 55);
    gtk_container_add(GTK_CONTAINER(eventbox), box);

    GtkWidget *avatar = NULL;
    GdkPixbuf *pixbuf = NULL;
    char *room_title = NULL;

    GtkWidget *title = gtk_label_new(NULL);
    mx_label_push_back(&labels_head, title, 38);
    gtk_widget_set_name(GTK_WIDGET(title), "room_title");
    gtk_widget_set_margin_top(GTK_WIDGET(title), 5);
    GtkWidget *preview = gtk_label_new(NULL);
    gtk_widget_set_name(GTK_WIDGET(preview), "room_text_preview");
    gtk_widget_set_margin_top(GTK_WIDGET(preview), 10);

    if (uid == 0) {
        if (THEME == 0)
            pixbuf = mx_get_pixbuf_with_size("client/img/standard/bookmark.png", 50, 50);
        else {
            pixbuf = mx_get_pixbuf_with_size("client/img/colored/bookmark.png", 50, 50);
        }
        room_title = mx_strdup(text_for_labels[38]);
        gtk_label_set_text(GTK_LABEL(title), room_title);
        char *tmp_preview = "@";
        tmp_preview = mx_strjoin(tmp_preview, t_user.pseudonim);
        gtk_label_set_text(GTK_LABEL(preview), tmp_preview);
        free(tmp_preview);

        avatar = gtk_drawing_area_new();
        gtk_widget_set_size_request(GTK_WIDGET(avatar), 50, 50);
        gtk_widget_set_margin_start(GTK_WIDGET(avatar), 15);
    }
    else {
        if (sockfd == -1){
            mx_connect_to_server(&sockfd);
        }

        // Change for image read from server!!
        mx_read_photo_from_bd((int)uid);
        //=====================================
        pixbuf = mx_get_pixbuf_with_size("client/img/tmp_avatar.png", 50, 50);
        avatar = gtk_drawing_area_new();
        gtk_widget_set_size_request(GTK_WIDGET(avatar), 50, 50);
        gtk_widget_set_margin_start(GTK_WIDGET(avatar), 15);
        remove("client/img/tmp_avatar.png");


        char sendBuff[256];
        bzero(sendBuff, 256);
        sprintf(sendBuff, "SendRoomData\n%d", uid);

        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &error, &len);
        if (retval != 0) {
            fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
            sockfd = -1;
            return NULL;
        }
        if (error != 0) {
            fprintf(stderr, "socket error: %s\n", strerror(error));
            sockfd = -1;
            return NULL;
        }

        if(send(sockfd, sendBuff, 256, 0) == -1){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return NULL;
        }
        
        char recvBuff[256];
        bzero(recvBuff, 256);
        if(recv(sockfd, recvBuff, 256, 0) == 0){
            perror("ERROR writing to socket");
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return NULL;
        }

        char **recvData = mx_strsplit(recvBuff, '\n');
        gtk_label_set_text(GTK_LABEL(title), recvData[0]);
        gtk_label_set_text(GTK_LABEL(preview), recvData[1]);
        room_title = mx_strdup(recvData[0]);
        mx_del_strarr(&recvData);
    }

    t_chats_list *node = NULL;
    if (mx_strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) > 0)
        node = mx_push_back_chat(&search_list_head, (int)uid, pixbuf, eventbox, room_title);
    else
        node = mx_push_back_chat(&chats_list_head, (int)uid, pixbuf, eventbox, room_title);
    
    if (uid != 0)
        g_signal_connect(G_OBJECT(avatar), "draw", G_CALLBACK(mx_draw_event_image_avatar), &node->avatar);
    else 
        g_signal_connect(G_OBJECT(avatar), "draw", G_CALLBACK(draw_image_square), &node->avatar);

    gtk_box_pack_start(GTK_BOX(box), avatar, FALSE, FALSE, 0);
    
    GtkWidget *v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(GTK_WIDGET(v_box), 15);
    gtk_box_pack_start(GTK_BOX(box), v_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(v_box), title, FALSE, FALSE, 0);
    gtk_widget_set_halign(GTK_WIDGET(title), GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(v_box), preview, FALSE, FALSE, 0);
    gtk_widget_set_halign(GTK_WIDGET(preview), GTK_ALIGN_START);

    return eventbox;
}
