#include "../../inc/uchat_client.h"

static void list_match_users(GtkWidget *widget, GdkEvent *event);
static void search_menu_click(GtkWidget *widget, GdkEventButton *event, 
    GdkDisplay *display);
static void create_search_menu(GtkWidget *entry, GdkEvent *event, 
    unsigned int *users_arr, int users_len);
static void search_room_click(GtkWidget *widget, GdkEventButton *event, gpointer uid);

void mx_configure_left_header(void) {
    GtkWidget *left_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_fixed_put(GTK_FIXED(chat_area), left_header, 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(left_header), 8);
    gtk_widget_set_size_request(GTK_WIDGET(left_header), L_FIELD_WIDTH, 23);

    settings_image.box = gtk_event_box_new();

    gtk_widget_set_valign(GTK_WIDGET(settings_image.box), GTK_ALIGN_END);
    gtk_widget_set_size_request(GTK_WIDGET(settings_image.box), 30, 30);
    gtk_widget_set_name(GTK_WIDGET(settings_image.box), "settings_image");


    gtk_box_pack_start(GTK_BOX(left_header),
        settings_image.box, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(settings_image.box), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(settings_image.box), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(settings_image.box), "button_press_event",
        G_CALLBACK(settings_click), NULL);

    
    entry_search = gtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(entry_search), "entry");
    gtk_box_pack_start(GTK_BOX(left_header), entry_search, TRUE, TRUE, 0);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_search), text_for_labels[13]);
    gtk_entry_set_max_length(GTK_ENTRY(entry_search), 20);
    gtk_widget_grab_focus(GTK_WIDGET(chat_area));
    g_signal_connect(G_OBJECT(entry_search), "changed", 
        G_CALLBACK(list_match_users), NULL);
}

static void list_match_users(GtkWidget *widget, GdkEvent *event) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }
    int len = mx_strlen(gtk_entry_get_text(GTK_ENTRY(widget)));
    if (len == 0) {
        mx_destroy_popups();
        return;
    }

    char sendBuff[1024];
    bzero(sendBuff, 1024);
    sprintf(sendBuff, "SearchInit\n%s\n%d", gtk_entry_get_text(GTK_ENTRY(widget)), t_user.id);
    
    int error = 0;
    socklen_t len1 = sizeof (error);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &error, &len1);
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

    if(send(sockfd, sendBuff, 1024, 0) == -1){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    int users_len = 0;
    if(recv(sockfd, &users_len, sizeof(int), 0) == 0){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    unsigned int *users_arr = NULL;
    if (users_len > 0)
        users_arr = (unsigned int *)malloc(users_len);
    else {
        if (search_list_head != NULL)
            mx_clear_chat_list(&search_list_head);
    }
    for (int i = 0; i < users_len; i++)
        if(recv(sockfd, &users_arr[i], sizeof(unsigned int), 0) == 0){
            pthread_t thread_id;
            char *err_msg = "Connection lost\nTry again later";
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
            sockfd = -1;
            return;
        }

    create_search_menu(widget, event, users_arr, users_len);

    if (users_len > 0)
        free(users_arr);
}



// Click on the window to close the menu
static void search_menu_click(GtkWidget *widget, GdkEventButton *event, GdkDisplay *display) {
    if (search_menu != NULL) {
        GtkAllocation alloc;
        gtk_widget_get_allocation(GTK_WIDGET(search_menu), &alloc);
        gint x_search, y_search;
        gtk_window_get_position(GTK_WINDOW(search_menu), &x_search, &y_search);
        selected_msg_widget = NULL;
        selected_msg_struct = NULL;
        if (event->x_root > x_search + alloc.width || event->y_root > y_search +alloc.height
            || event->x_root < x_search || event->y_root < y_search) {    
            mx_destroy_popups();
            gtk_widget_grab_focus(GTK_WIDGET(chat_area));
        }
    }
}

static void create_search_menu(GtkWidget *entry, GdkEvent *event, 
    unsigned int *users_arr, int users_len) {

    // Create window and grab focus of the mouse
    //==================================================================================
    if (search_menu == NULL) {
        search_menu = gtk_window_new(GTK_WINDOW_POPUP);
        gtk_widget_set_device_events(GTK_WIDGET(search_menu), 
            gtk_get_current_event_device(), GDK_ALL_EVENTS_MASK);
        gtk_widget_realize(GTK_WIDGET(search_menu));
        gtk_window_set_resizable(GTK_WINDOW(search_menu), FALSE);
        gtk_window_set_decorated(GTK_WINDOW(search_menu), FALSE);

        gdk_window_show(gtk_widget_get_window(GTK_WIDGET(search_menu)));
        GdkDisplay *display = gtk_widget_get_display(GTK_WIDGET(chat_area));
        GdkCursor *cursor = gdk_cursor_new_from_name (display, "default");
        GdkGrabStatus status = gdk_seat_grab(gdk_display_get_default_seat(display),
            gtk_widget_get_window(GTK_WIDGET(window)), GDK_SEAT_CAPABILITY_ALL_POINTING, TRUE,
            cursor, event, NULL, NULL);
        g_object_unref(G_OBJECT(cursor));    
        g_signal_connect(G_OBJECT(window), "button_press_event",
            G_CALLBACK(search_menu_click), display);
    }
    //==================================================================================

    // Fill the search window
    //==================================================================================
    gtk_container_forall(GTK_CONTAINER(search_menu), (GtkCallback)gtk_widget_destroy, NULL);
    GtkAllocation win_alloc;
    GtkAllocation entry_alloc;
    gtk_widget_get_allocation(GTK_WIDGET(search_menu), &win_alloc);
    gtk_widget_get_allocation(GTK_WIDGET(entry), &entry_alloc);

    GtkAdjustment *vadjustment = gtk_adjustment_new(0, 0, 
        win_alloc.height, 100, 100, win_alloc.height);
    GtkWidget *search_scroll_container = gtk_scrolled_window_new(NULL, vadjustment);
    gtk_widget_set_size_request(GTK_WIDGET(search_scroll_container), 
        entry_alloc.width, win_alloc.height);
    gtk_container_add(GTK_CONTAINER(search_menu), search_scroll_container);

    GtkWidget *search_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(search_scroll_container), search_container);
    gtk_widget_set_name(GTK_WIDGET(search_scroll_container), "search_menu");

    for (int i = 0; i < users_len; i++) {
        gtk_box_pack_start(GTK_BOX(search_container), 
            mx_create_room(users_arr[i], entry_alloc.width, search_room_click), FALSE, FALSE, 0);
    }

    //==================================================================================

    gint x_win;
    gint y_win;
    gtk_window_get_position(GTK_WINDOW(window), &x_win, &y_win);

    gtk_window_move(GTK_WINDOW(search_menu), x_win + entry_alloc.x, y_win + 77);
    gtk_widget_show_all(GTK_WIDGET(search_menu));
}

static void search_room_click(GtkWidget *widget, GdkEventButton *event, gpointer uid) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {        
        t_chats_list *node = mx_chat_search(&chats_list_head, (int)(uintptr_t)uid);

        if (node != NULL) {
            gtk_entry_set_text(GTK_ENTRY(entry_search), "");
            gtk_widget_set_can_focus(GTK_WIDGET(chat_area), TRUE);
            gtk_widget_grab_focus(GTK_WIDGET(chat_area));
            room_click(widget, event, uid);
            return;
        }
        gtk_entry_set_text(GTK_ENTRY(entry_search), "");

        GtkWidget *room = mx_create_room((unsigned int)(uintptr_t)uid, L_FIELD_WIDTH, room_click);
        gtk_box_pack_start(GTK_BOX(chats_list), room, FALSE, FALSE, 0);
        gtk_widget_show_all(GTK_WIDGET(room));
        
        room_click(room, event, uid);
        gtk_widget_set_can_focus(GTK_WIDGET(chat_area), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(chat_area));
    }
}
