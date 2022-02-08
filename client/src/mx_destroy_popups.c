#include "../inc/uchat_client.h"

int mx_destroy_popups(void) {
    int status = 0;
    if (tools_menu != NULL) {
        gdk_seat_ungrab(gdk_display_get_default_seat(gdk_display_get_default()));
        gtk_widget_destroy(GTK_WIDGET(tools_menu));
        tools_menu = NULL;
        status = 1;
    }
    if (error_revealer != NULL) {
        gtk_container_forall(GTK_CONTAINER(error_revealer), (GtkCallback)gtk_widget_destroy, NULL);
        gtk_widget_destroy(GTK_WIDGET(error_revealer));
        error_revealer = NULL;
        status = 1;
    }
    if (search_menu != NULL) {
        gdk_seat_ungrab(gdk_display_get_default_seat(gdk_display_get_default()));
        gtk_widget_destroy(GTK_WIDGET(search_menu));
        search_menu = NULL;
        if (search_list_head != NULL)
            mx_clear_chat_list(&search_list_head);
        status = 1;
    }
    if (t_chat_room_vars.more_box != NULL) {
        //gtk_widget_destroy(GTK_WIDGET(t_chat_room_vars.more_box));
        //t_chat_room_vars.more_box = NULL;

        gdk_seat_ungrab(gdk_display_get_default_seat(gdk_display_get_default()));
        gtk_widget_destroy(GTK_WIDGET(t_chat_room_vars.more_box));
        t_chat_room_vars.more_box = NULL;

        GList *children = gtk_container_get_children(GTK_CONTAINER(t_chat_room_vars.message_enter_area));
        gtk_widget_unset_state_flags(GTK_WIDGET(g_list_nth_data(children, 4)), 
            GTK_STATE_FLAG_PRELIGHT);
        gtk_widget_unset_state_flags(GTK_WIDGET(g_list_nth_data(children, 4)), 
            GTK_STATE_FLAG_CHECKED);
        g_list_free(children);
        status = 1;
    }
    return status;
}
