#include "../../inc/uchat_client.h"

// Add button
//=================================================================================
void mx_attach(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry) {
    mx_destroy_popups();
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        GtkWidget *dialog;
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
        gint res;

        dialog = gtk_file_chooser_dialog_new ("Select an Image",
                                            GTK_WINDOW(window),
                                            action,
                                            "_Cancel",
                                            GTK_RESPONSE_CANCEL,
                                            "_Select",
                                            GTK_RESPONSE_ACCEPT,
                                            NULL);

        GtkFileFilter *filter = gtk_file_filter_new();
        char *pattern_arr[13] = {"*.tif", "*.tiff", "*.bmp", "*.jpg", "*.jpeg", "*.gif",
            "*.png", "*.eps", "*.raw", "*.cr2", "*.nef", "*.orf", "*.sr2"};
        for (int i = 0; i < 13; ++i)
            gtk_file_filter_add_pattern(filter, pattern_arr[i]);
        gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);

        res = gtk_dialog_run (GTK_DIALOG (dialog));
        char *filename = NULL;
        if (res == GTK_RESPONSE_ACCEPT)
        {
            GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
            filename = gtk_file_chooser_get_filename (chooser);
        }
        gtk_widget_destroy (dialog);

        if (filename != NULL) {
            struct stat buf;
            stat(filename, &buf);
            if (buf.st_size < 5242880) // < 5mb
                mx_create_attach_form(entry, filename);
            else {
                pthread_t thread_id;
                char *err_msg = "The file is too big!";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            }
        }
    }
}

void mx_attach_send_message_on_enter(GtkWidget *widget, void **arr) {
    GdkPixbuf *pixbuf = arr[1];
    time_t curtime;
    time(&curtime);

    char *text = NULL;
    if (mx_strlen(gtk_entry_get_text(GTK_ENTRY(widget))) > 0)
        text = strdup(gtk_entry_get_text(GTK_ENTRY(widget)));
    
    t_message *msg = NULL;
    char sendBuff[2056];
    bzero(sendBuff, 2056);
    if (gdk_pixbuf_get_width(GDK_PIXBUF(pixbuf)) > 350) {
        sprintf(sendBuff, "InsertMessage\n%u\n%u\n%lu\n(null)",
                t_user.id, curr_destination, curtime);
        
        if(send(sockfd, sendBuff, 2056, 0) == -1){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return;
        }

        int m_id = 0;
        if(recv(sockfd, &m_id, sizeof(int), 0) == 0){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return;
        }
        max_msg_id = m_id;

        msg = mx_push_back_message(&curr_room_msg_head,
            NULL, 
            t_user.id, 
            pixbuf,
            curtime,
            max_msg_id);
        mx_add_message(t_chat_room_vars.messages_box, msg);

        mx_write_image_message((char *)arr[0], msg->id);

        if (text != NULL) {
            bzero(sendBuff, 2056);
            sprintf(sendBuff, "InsertMessage\n%u\n%u\n%lu\n%s",
                    t_user.id, curr_destination, curtime, text);
            
            if(send(sockfd, sendBuff, 2056, 0) == -1){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return;
            }

            int m_id = 0;
            if(recv(sockfd, &m_id, sizeof(int), 0) == 0){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return;
            }
            max_msg_id = m_id;

            msg = mx_push_back_message(&curr_room_msg_head,
                text, 
                t_user.id, 
                NULL,
                curtime,
                max_msg_id);
            mx_add_message(t_chat_room_vars.messages_box, msg);
        }
    }
    else {
        bzero(sendBuff, 2056);
        if (text == NULL)
            text = mx_strdup("(null)");
        sprintf(sendBuff, "InsertMessage\n%u\n%u\n%lu\n%s",
                t_user.id, curr_destination, curtime, text);

        if (!mx_strcmp(text, "(null)")) {
            free(text);
            text = NULL;
        }
        
        if(send(sockfd, sendBuff, 2056, 0) == -1){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return;
        }

        int m_id = 0;
        if(recv(sockfd, &m_id, sizeof(int), 0) == 0){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return;
        }
        max_msg_id = m_id;

        msg = mx_push_back_message(&curr_room_msg_head,
            text, 
            t_user.id, 
            pixbuf,
            curtime,
            max_msg_id);
        mx_add_message(t_chat_room_vars.messages_box, msg);

        mx_write_image_message((char *)arr[0], msg->id);
    }

    gtk_widget_destroy(GTK_WIDGET(blackout));
    blackout = NULL;
    g_object_unref(arr[2]);
    g_free(arr);

    gtk_widget_set_can_focus(GTK_WIDGET(chat_area), TRUE);
    gtk_widget_grab_focus(GTK_WIDGET(chat_area));
}
//=================================================================================

// Message entry field
//=================================================================================
void entry_chat_fill_event(GtkWidget *widget, GdkEvent *event) {
    mx_destroy_popups();
    int len = strlen(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widget))));
    GList *children = gtk_container_get_children(GTK_CONTAINER(t_chat_room_vars.message_enter_area));
    if (len > 0) {
        gtk_widget_hide(GTK_WIDGET(g_list_nth_data(children, 2)));
        gtk_widget_show(GTK_WIDGET(g_list_nth_data(children, 3)));
    }
    else {
        gtk_widget_hide(GTK_WIDGET(g_list_nth_data(children, 3)));
        gtk_widget_show(GTK_WIDGET(g_list_nth_data(children, 2)));
    }
    g_list_free(children);
}

void mx_send_message_on_enter(GtkWidget *widget) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }

    if (mx_strlen(gtk_entry_get_text(GTK_ENTRY(widget))) > 0) {
        if (edit_prev == NULL) {
            time_t curtime;
            time(&curtime);

            char sendBuff[2056];
            bzero(sendBuff, 2056);
            sprintf(sendBuff, "InsertMessage\n%u\n%u\n%lu\n%s",
                    t_user.id, curr_destination, curtime, gtk_entry_get_text(GTK_ENTRY(widget)));
            
            if(send(sockfd, sendBuff, 2056, 0) == -1){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return;
            }

            int m_id = 0;
            if(recv(sockfd, &m_id, sizeof(int), 0) == 0){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return;
            }
            max_msg_id = m_id;
            
            t_message *msg = mx_push_back_message(&curr_room_msg_head,
                strdup(gtk_entry_get_text(GTK_ENTRY(widget))), 
                t_user.id, 
                NULL,
                curtime,
                m_id);
            mx_add_message(t_chat_room_vars.messages_box, msg);

            gtk_entry_set_text(GTK_ENTRY(widget), "");
            t_chats_list *node = chats_list_head;
            while (node->uid != (int)curr_destination)
                node = node->next;
            gtk_box_reorder_child(GTK_BOX(chats_list), node->room, 0);
        }
        else {
            char *text = mx_strdup(gtk_entry_get_text(GTK_ENTRY(widget)));

            GList *children = gtk_container_get_children(GTK_CONTAINER(selected_msg_widget));
            GList *box_children = gtk_container_get_children(GTK_CONTAINER(g_list_nth_data(children, 0)));
            guint size = g_list_length(box_children);
            gtk_label_set_text(GTK_LABEL(g_list_nth_data(box_children, size - 2)), text);
            g_list_free(children);
            g_list_free(box_children);

            free(selected_msg_struct->text);
            selected_msg_struct->text = text;
            
            gtk_widget_destroy(GTK_WIDGET(edit_prev));
            edit_prev = NULL;

            gtk_entry_set_text(GTK_ENTRY(widget), "");
            gtk_widget_set_can_focus(GTK_WIDGET(widget), TRUE);
            gtk_widget_grab_focus(GTK_WIDGET(widget));

            char sendBuff[256];
            bzero(sendBuff, 256);
            sprintf(sendBuff, "EditMessage\n%d\n%d\n%d\n%s", t_user.id, (int)curr_destination,
                (int)selected_msg_struct->id, text);


            if(send(sockfd, sendBuff, 256, 0) == -1){
                pthread_t thread_id;
                char *err_msg = "Connection lost\nTry again later";
                pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                sockfd = -1;
                return;
            }
        }
    }
}
//=================================================================================

// Tick button
//=================================================================================
void mx_send_message(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }

    mx_destroy_popups();
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        if (mx_strlen(gtk_entry_get_text(GTK_ENTRY(entry))) > 0) {
            if (edit_prev == NULL) {
                time_t curtime;
                time(&curtime);

                char sendBuff[2056];
                bzero(sendBuff, 2056);
                sprintf(sendBuff, "InsertMessage\n%u\n%u\n%lu\n%s",
                    t_user.id, curr_destination, curtime, gtk_entry_get_text(GTK_ENTRY(entry)));
                
                int error = 0;
                socklen_t len = sizeof(error);
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
                
                if(send(sockfd, sendBuff, 2056, 0) == -1){
                    pthread_t thread_id;
                    char *err_msg = "Connection lost\nTry again later";
                    pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                    sockfd = -1;
                    return;
                }


                int m_id = 0;
                if(recv(sockfd, &m_id, sizeof(int), 0) == 0){
                    pthread_t thread_id;
                    char *err_msg = "Connection lost\nTry again later";
                    pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                    sockfd = -1;
                    return;
                }
                max_msg_id = m_id;

                t_message *msg = mx_push_back_message(&curr_room_msg_head,
                    strdup(gtk_entry_get_text(GTK_ENTRY(entry))), 
                    t_user.id, 
                    NULL,
                    curtime,
                    m_id);
                mx_add_message(t_chat_room_vars.messages_box, msg);

                gtk_entry_set_text(GTK_ENTRY(entry), "");
                t_chats_list *node = chats_list_head;
                while (node->uid != (int)curr_destination)
                    node = node->next;
                gtk_box_reorder_child(GTK_BOX(chats_list), node->room, 0);
            }
            else {
                char *text = mx_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));

                GList *children = gtk_container_get_children(GTK_CONTAINER(selected_msg_widget));
                GList *box_children = gtk_container_get_children(GTK_CONTAINER(g_list_nth_data(children, 0)));
                guint size = g_list_length(box_children);
                gtk_label_set_text(GTK_LABEL(g_list_nth_data(box_children, size - 2)), text);
                g_list_free(children);
                g_list_free(box_children);

                free(selected_msg_struct->text);
                selected_msg_struct->text = text;
                
                gtk_widget_destroy(GTK_WIDGET(edit_prev));
                edit_prev = NULL;

                gtk_entry_set_text(GTK_ENTRY(entry), "");
                gtk_widget_set_can_focus(GTK_WIDGET(entry), TRUE);
                gtk_widget_grab_focus(GTK_WIDGET(entry));

                char sendBuff[256];
                bzero(sendBuff, 256);
                sprintf(sendBuff, "EditMessage\n%d\n%d\n%d\n%s", t_user.id, (int)curr_destination,
                    (int)selected_msg_struct->id, text);


                if(send(sockfd, sendBuff, 256, 0) == -1){
                    pthread_t thread_id;
                    char *err_msg = "Connection lost\nTry again later";
                    pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
                    sockfd = -1;
                    return;
                }
            }
        }
    }
}
//=================================================================================

// More button
//=================================================================================
static void create_scrolled_more(GtkWidget *clicked, GtkWidget *grid);

void more_content_click(GtkWidget *widget, GdkEventButton *event, GtkWidget *data) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        GList *children = gtk_container_get_children(GTK_CONTAINER(widget));
        GtkWidget *selected_obj = g_list_nth_data(children, 0);
        GtkStateFlags flags = gtk_widget_get_state_flags(GTK_WIDGET(selected_obj));
        g_list_free(children);
        if (!(flags & GTK_STATE_FLAG_CHECKED)) {

            GtkWidget *first_eventbox = gtk_grid_get_child_at(GTK_GRID(data), 1, 2);
            GtkWidget *second_eventbox = gtk_grid_get_child_at(GTK_GRID(data), 3, 2);

            children = gtk_container_get_children(GTK_CONTAINER(first_eventbox));
            flags = gtk_widget_get_state_flags(GTK_WIDGET(g_list_nth_data(children, 0)));
            g_list_free(children);
            if (flags & GTK_STATE_FLAG_CHECKED) {
                gtk_container_forall(GTK_CONTAINER(first_eventbox), 
                    (GtkCallback)gtk_widget_unset_state_flags, (gpointer)(intptr_t)GTK_STATE_FLAG_CHECKED);
            }
            else {
                gtk_container_forall(GTK_CONTAINER(second_eventbox), 
                    (GtkCallback)gtk_widget_unset_state_flags, (gpointer)(intptr_t)GTK_STATE_FLAG_CHECKED);
            }

            gtk_widget_set_state_flags(GTK_WIDGET(selected_obj), GTK_STATE_FLAG_CHECKED, TRUE);
            create_scrolled_more(widget, data);
        }
    }
}

static void more_win_click(GtkWidget *widget, GdkEventButton *event, GdkDisplay *display);
static void create_more_window(GdkEvent *event);

void mx_more_click(GtkWidget *widget, GdkEvent *event) {
    if (((GdkEventButton *)event)->type == GDK_BUTTON_PRESS && ((GdkEventButton *)event)->button == 1) {
        GtkStateFlags flags = gtk_widget_get_state_flags(GTK_WIDGET(widget));
        if (!(flags & GTK_STATE_FLAG_CHECKED)) {
            mx_destroy_popups();
            gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_CHECKED, FALSE);

            create_more_window(event);
            gtk_widget_set_name(GTK_WIDGET(t_chat_room_vars.more_box), "more_box");

            GtkWidget *more_grid = gtk_grid_new();
            gtk_container_add(GTK_CONTAINER(t_chat_room_vars.more_box), more_grid);
            gtk_widget_set_valign(GTK_WIDGET(more_grid), GTK_ALIGN_CENTER);
            gtk_widget_set_size_request(GTK_WIDGET(more_grid), 300, 480);
            
            //gtk_widget_set_margin_top(GTK_WIDGET(more_grid), 10);
            //gtk_widget_set_margin_bottom(GTK_WIDGET(more_grid), 10);

            GtkWidget *doggy_event_box = gtk_event_box_new();
            GtkWidget *doggy_label = gtk_label_new("EMOJI");
            gtk_container_add(GTK_CONTAINER(doggy_event_box), doggy_label);
            gtk_widget_set_size_request(GTK_WIDGET(doggy_label), 150, 30);
            gtk_widget_set_name(GTK_WIDGET(doggy_label), "stickers_label");
            g_signal_connect(G_OBJECT(doggy_event_box), "button_press_event", 
                G_CALLBACK(more_content_click), more_grid);
            gtk_grid_attach(GTK_GRID(more_grid), doggy_event_box, 1, 2, 1, 1);

            GtkWidget *kitty_event_box = gtk_event_box_new();
            GtkWidget *kitty_label = gtk_label_new("FUNNY");
            gtk_container_add(GTK_CONTAINER(kitty_event_box), kitty_label);
            gtk_widget_set_size_request(GTK_WIDGET(kitty_label), 150, 30);
            gtk_widget_set_name(GTK_WIDGET(kitty_label), "stickers_label");
            g_signal_connect(G_OBJECT(kitty_event_box), "button_press_event", 
                G_CALLBACK(more_content_click), more_grid);
            gtk_grid_attach(GTK_GRID(more_grid), kitty_event_box, 3, 2, 1, 1);


            GtkWidget *horizontal_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
            gtk_widget_set_size_request(GTK_WIDGET(horizontal_separator), 300, 0);
            gtk_grid_attach(GTK_GRID(more_grid), horizontal_separator, 1, 3, 3, 1);
            gtk_widget_set_name(GTK_WIDGET(horizontal_separator), "separator");

            gtk_window_set_keep_above(GTK_WINDOW(t_chat_room_vars.more_box), TRUE);
            gtk_widget_show_all(GTK_WIDGET(t_chat_room_vars.more_box));
        }
        else {
            gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_CHECKED);

            gtk_widget_destroy(GTK_WIDGET(t_chat_room_vars.more_box));
            t_chat_room_vars.more_box = NULL;
        }
    }
}

// Click on the window to close the menu
static void more_win_click(GtkWidget *widget, GdkEventButton *event, GdkDisplay *display) {
    if (t_chat_room_vars.more_box != NULL) {
        GtkAllocation alloc;
        gtk_widget_get_allocation(GTK_WIDGET(t_chat_room_vars.more_box), &alloc);
        gint x_more, y_more;
        gtk_window_get_position(GTK_WINDOW(t_chat_room_vars.more_box), &x_more, &y_more);
        selected_msg_widget = NULL;
        selected_msg_struct = NULL;
        if (event->x_root > x_more + alloc.width || event->y_root > y_more +alloc.height
            || event->x_root < x_more || event->y_root < y_more) {    
            mx_destroy_popups();
        }
    }
}

static void create_more_window(GdkEvent *event) {
    t_chat_room_vars.more_box = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_set_device_events(GTK_WIDGET(t_chat_room_vars.more_box), gtk_get_current_event_device(), GDK_ALL_EVENTS_MASK);
    gtk_widget_realize(GTK_WIDGET(t_chat_room_vars.more_box));
    gtk_window_set_resizable(GTK_WINDOW(t_chat_room_vars.more_box), FALSE);
    gtk_window_set_decorated(GTK_WINDOW(t_chat_room_vars.more_box), FALSE);

    gdk_window_show(gtk_widget_get_window(GTK_WIDGET(t_chat_room_vars.more_box)));
    GdkDisplay *display = gtk_widget_get_display(GTK_WIDGET(chat_area));
    GdkCursor *cursor = gdk_cursor_new_from_name (display, "default");
    GdkGrabStatus status = gdk_seat_grab(gdk_display_get_default_seat(display),
        gtk_widget_get_window(GTK_WIDGET(window)), GDK_SEAT_CAPABILITY_ALL_POINTING, TRUE,
        cursor, event, NULL, NULL);
    g_object_unref(G_OBJECT(cursor));    
    g_signal_connect(G_OBJECT(window), "button_press_event",
        G_CALLBACK(more_win_click), display);

    // Move window of tools to the mouse click position
    gint x_win, y_win;
    gtk_window_get_position(GTK_WINDOW(window), &x_win, &y_win);
    gtk_window_move(GTK_WINDOW(t_chat_room_vars.more_box), 
        x_win + CUR_WIDTH + 10, y_win + 200);
}

static void sticker_click(GtkWidget *widget, GdkEventButton *event, gpointer path) {
    path = (char *)path;
    GdkPixbuf *pixbuf = mx_create_pixbuf(path);
    
    time_t curtime;
    time(&curtime);

    char sendBuff[2056];
    bzero(sendBuff, 2056);
    sprintf(sendBuff, "InsertMessage\n%u\n%u\n%lu\n(null)",
            t_user.id, curr_destination, curtime);
    
    if(send(sockfd, sendBuff, 2056, 0) == -1){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    int m_id = 0;
    if(recv(sockfd, &m_id, sizeof(int), 0) == 0){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }
    max_msg_id = m_id;

    t_message *msg = mx_push_back_message(&curr_room_msg_head,
        NULL, 
        t_user.id, 
        pixbuf,
        curtime,
        max_msg_id);
    mx_add_message(t_chat_room_vars.messages_box, msg);

    mx_write_image_message(path, msg->id);
    free(path);
}

static void create_scrolled_more(GtkWidget *clicked, GtkWidget *grid) {
    if (gtk_grid_get_child_at(GTK_GRID(grid), 1, 4) != NULL)
        gtk_widget_destroy(GTK_WIDGET(gtk_grid_get_child_at(GTK_GRID(grid), 1, 4)));

    GtkAdjustment *vadjustment = gtk_adjustment_new(0, 0, 
        300, 100, 100, 402);
    GtkWidget *more_scroll = gtk_scrolled_window_new(NULL, vadjustment);
    gtk_widget_set_size_request(GTK_WIDGET(more_scroll), 280, 402);
    gtk_grid_attach(GTK_GRID(grid), more_scroll, 1, 4, 3, 1);

    GtkWidget *stickers_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(more_scroll), stickers_grid);
    gtk_widget_set_valign(GTK_WIDGET(stickers_grid), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(stickers_grid), 280, 0);

    DIR *dir;
    struct dirent *sd;
    char path[PATH_MAX];
    mx_memcpy(path, "client/img/stickers/", 21);
    if (clicked == gtk_grid_get_child_at(GTK_GRID(grid), 1, 2)) {
        dir = opendir("client/img/stickers/Emoji");
        mx_memcpy(&path[20], "Emoji/", 7);
    }
    else {
        dir = opendir("client/img/stickers/Funny");
        mx_memcpy(&path[20], "Funny/", 7);
    }

    for (int i = 1; ; i++) {
        int status = 0;
        for (int j = 1; j < 4; j++) {
            sd = readdir(dir);
            if (sd == NULL) {
                status = -1;
                break;
            }
            if (mx_strlen(sd->d_name) < 3) {
                j--;
                continue;
            }
            char *tmp_path = mx_strjoin(path, sd->d_name);
            GdkPixbuf *tmp_pixbuf = mx_get_pixbuf_with_size(tmp_path, 95, 95);
            GtkWidget *eventbox_image = gtk_event_box_new();
            g_signal_connect(G_OBJECT(eventbox_image), "button_press_event",
                G_CALLBACK(sticker_click), (gpointer)mx_strdup(tmp_path));
            GtkWidget *image = gtk_image_new_from_pixbuf(tmp_pixbuf);
            g_object_unref(G_OBJECT(tmp_pixbuf));
            free(tmp_path);
            gtk_widget_set_size_request(GTK_WIDGET(image), 20, 20);
            gtk_container_add(GTK_CONTAINER(eventbox_image), image);

            gtk_grid_attach(GTK_GRID(stickers_grid), eventbox_image, j, i, 1, 1);
        }
        if (status == -1)
            break;
    }

    gtk_widget_show_all(GTK_WIDGET(more_scroll));
}
//=================================================================================
