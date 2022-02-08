#include "../../inc/uchat_client.h"

static void mx_themes_close(GtkWidget *widget, GdkEventButton *event);
static void mx_theme_eventbox_click(GtkWidget *widget, GdkEventButton *event, char *name);
GtkWidget *mx_theme_create_box(char *path, char *name);

GtkWidget *mx_theme_create_box(char *path, char *name) {
    GtkWidget *eventbox = gtk_event_box_new();
    gtk_widget_set_size_request(GTK_WIDGET(eventbox), 200, 100);
    gtk_widget_set_name(GTK_WIDGET(eventbox), "language_choose_eventbox");
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(eventbox), box);
    GdkPixbuf *pixbuf = mx_get_pixbuf_with_size(path, 50, 50);
    GtkWidget *preview = gtk_image_new_from_pixbuf(GDK_PIXBUF(pixbuf));
    g_object_unref(G_OBJECT(pixbuf));
    gtk_box_pack_start(GTK_BOX(box), preview, FALSE, FALSE, 0);
    GtkWidget *label = gtk_label_new(name);
    gtk_widget_set_name(GTK_WIDGET(label), "language_label");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, FALSE, 0);

    if (THEME == 0) {
        if (!mx_strcmp(name, "Dark") && THEME == 0)
            pixbuf = mx_get_pixbuf_with_size("client/img/standard/radiobox_checked.png", 20, 20);
        else if (!mx_strcmp(name, "Light") && THEME == 1)
            pixbuf = mx_get_pixbuf_with_size("client/img/standard/radiobox_checked.png", 20, 20);
        else
            pixbuf = mx_get_pixbuf_with_size("client/img/standard/radiobox.png", 20, 20);
    }
    else {
        if (!mx_strcmp(name, "Light") && THEME == 0)
            pixbuf = mx_get_pixbuf_with_size("client/img/colored/radiobox_checked.png", 20, 20);
        else if (!mx_strcmp(name, "Light") && THEME == 1)
            pixbuf = mx_get_pixbuf_with_size("client/img/colored/radiobox_checked.png", 20, 20);
        else
            pixbuf = mx_get_pixbuf_with_size("client/img/colored/radiobox.png", 20, 20);
    }

    GtkWidget *radiobox = gtk_image_new_from_pixbuf(GDK_PIXBUF(pixbuf));
    g_object_unref(G_OBJECT(pixbuf));
    gtk_box_pack_start(GTK_BOX(box), radiobox, FALSE, FALSE, 20);
    gtk_widget_set_halign(GTK_WIDGET(radiobox), GTK_ALIGN_CENTER);

    g_signal_connect(G_OBJECT(eventbox), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(eventbox), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(eventbox), "button_press_event",
        G_CALLBACK(mx_theme_eventbox_click), name);

    return eventbox;
}

void mx_create_theme_menu(void) {
    // Create a blackout - a dark background behind the form
    //==================================================================================
    blackout = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(blackout), "blackout");    
    gtk_fixed_put(GTK_FIXED(chat_area), blackout, 0, 0);
    gtk_widget_set_size_request(GTK_WIDGET(blackout), CUR_WIDTH, CUR_HEIGHT);
    //==================================================================================

    GtkWidget *themes_form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(GTK_WIDGET(themes_form), 400, 100);
    gtk_widget_set_halign(GTK_WIDGET(themes_form), GTK_ALIGN_CENTER);
    gtk_widget_set_valign(GTK_WIDGET(themes_form), GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(blackout), themes_form);
    gtk_widget_set_name(GTK_WIDGET(themes_form), "edit_user_form");
    g_signal_connect(G_OBJECT(blackout), "button_press_event",
        G_CALLBACK(blackout_destroy), themes_form);
    

    // "close" image
    //==================================================================================
    GtkWidget *close_image_box = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(close_image_box), "close_image_box");
    gtk_widget_set_size_request(GTK_WIDGET(close_image_box), 25, 25);

    g_signal_connect(G_OBJECT(close_image_box), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(close_image_box), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(close_image_box), "button_press_event",
        G_CALLBACK(mx_themes_close), NULL);
    //==================================================================================

    
    // Create and pack widgets
    GtkWidget *themes_chooser = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(themes_form), themes_chooser, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(themes_chooser),
        mx_theme_create_box("client/img/standard-theme.png", "Dark"),
        FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(themes_chooser),
        mx_theme_create_box("client/img/colored-theme.png", "Light"),
        FALSE, FALSE, 0);

    gtk_widget_show_all(GTK_WIDGET(blackout));
}

static void mx_themes_close(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_widget_destroy(GTK_WIDGET(blackout));
        blackout = NULL;
    }
}

static void mx_theme_eventbox_click(GtkWidget *widget, GdkEventButton *event, char *name) {
    if (!mx_strcmp(name, "Dark")) {
        THEME = 0;
        g_object_unref(G_OBJECT(cssProvider));
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "client/css/standard/uchat.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        
        t_chats_list *node = mx_chat_search(&chats_list_head, 0);

        GList *box = gtk_container_get_children(GTK_CONTAINER(node->room));
        GList *children = gtk_container_get_children(GTK_CONTAINER(g_list_nth_data(box, 0)));
        g_list_free(box);
        g_signal_handlers_disconnect_by_data(G_OBJECT(g_list_nth_data(children, 0)), &node->avatar);

        g_object_unref(node->avatar);
        node->avatar = mx_get_pixbuf_with_size("client/img/standard/bookmark.png", 50, 50);
        g_signal_connect(G_OBJECT(g_list_nth_data(children, 0)), "draw", 
            G_CALLBACK(mx_draw_event_image_avatar), &node->avatar);

        gtk_widget_queue_draw(GTK_WIDGET(chat_area));
        g_list_free(children);
    }
    else {
        THEME = 1;
        g_object_unref(G_OBJECT(cssProvider));
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "client/css/colored/uchat.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        t_chats_list *node = mx_chat_search(&chats_list_head, 0);
        GList *box = gtk_container_get_children(GTK_CONTAINER(node->room));
        GList *children = gtk_container_get_children(GTK_CONTAINER(g_list_nth_data(box, 0)));
        g_list_free(box);
        g_signal_handlers_disconnect_by_data(G_OBJECT(g_list_nth_data(children, 0)), &node->avatar);

        g_object_unref(node->avatar);
        node->avatar = mx_get_pixbuf_with_size("client/img/colored/bookmark.png", 50, 50);
        g_signal_connect(G_OBJECT(g_list_nth_data(children, 0)), "draw", 
            G_CALLBACK(mx_draw_event_image_avatar), &node->avatar);

        gtk_widget_queue_draw(GTK_WIDGET(chat_area));
        g_list_free(children);
    }

    char sendBuff[256];
    mx_memset(sendBuff, 0, 256);
    sprintf(sendBuff, "UpdateTheme\n%d\n%d", THEME, t_user.id);

    if(send(sockfd, sendBuff, 256, 0) == -1){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    gtk_widget_destroy(GTK_WIDGET(blackout));
    blackout = NULL;
}
