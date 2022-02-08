#include "../inc/uchat_client.h"

void mx_add_message(GtkWidget *container, t_message *message) {
    GtkWidget *msg = mx_create_message(message);
    GtkWidget *day_show = NULL;

    char *day_ch = mx_strndup(ctime(&message->seconds) + 8, 2);
    int cur_day_int = mx_atoi(day_ch);
    free(day_ch);

    int next_day_int = 0;

    if (message->next != NULL) {
        day_ch = mx_strndup(ctime(&message->next->seconds) + 8, 2);
        next_day_int = mx_atoi(day_ch);
        free(day_ch);
    }

    if (message->id == 1) {
        char *day_info = mx_strndup(ctime(&message->seconds) + 4, 7);
        day_info = mx_strjoin(day_info, ctime(&message->seconds) + 20);
        GtkWidget *first_date = gtk_label_new(day_info);
        free(day_info);
        gtk_widget_set_name(GTK_WIDGET(first_date), "day_info_show");
        gtk_widget_set_halign(GTK_WIDGET(first_date), GTK_ALIGN_CENTER);
        gtk_widget_set_margin_top(GTK_WIDGET(first_date), 5);
        gtk_box_pack_start(GTK_BOX(container), first_date, FALSE, FALSE, 0);
        gtk_widget_show_all(GTK_WIDGET(first_date));
    }
    else {
        day_ch = mx_strndup(ctime(&mx_message_search(&curr_room_msg_head, message->id - 1)->seconds) + 8, 2);
        int prev_day_int = mx_atoi(day_ch);
        free(day_ch);

        if (cur_day_int != prev_day_int) {
            char *day_info = mx_strndup(ctime(&message->seconds) + 4, 7);
            day_info = mx_strjoin(day_info, ctime(&message->seconds) + 20);
            GtkWidget *first_date = gtk_label_new(day_info);
            free(day_info);
            gtk_widget_set_name(GTK_WIDGET(first_date), "day_info_show");
            gtk_widget_set_halign(GTK_WIDGET(first_date), GTK_ALIGN_CENTER);
            gtk_widget_set_margin_top(GTK_WIDGET(first_date), 5);
            gtk_box_pack_start(GTK_BOX(container), first_date, FALSE, FALSE, 0);
            gtk_widget_show_all(GTK_WIDGET(first_date));
        }
    }

    if (message->uid == (unsigned int)t_user.id) {
        gtk_widget_set_halign(GTK_WIDGET(msg), GTK_ALIGN_END);
        gtk_widget_set_margin_end(GTK_WIDGET(msg), 7);
    }
    else {
        gtk_widget_set_halign(GTK_WIDGET(msg), GTK_ALIGN_START);
        gtk_widget_set_margin_start(GTK_WIDGET(msg), 5);
    }

    gtk_widget_set_margin_top(GTK_WIDGET(msg), 5);

    gtk_box_pack_start(GTK_BOX(container), msg, FALSE, FALSE, 0); 

    gtk_widget_show_all(GTK_WIDGET(msg));
}
