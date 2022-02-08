#include "../../inc/uchat_client.h"

void mx_create_message_enter_area(void) {
    GtkWidget *entry_chat = gtk_entry_new();

    t_chat_room_vars.message_enter_area = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(GTK_WIDGET(t_chat_room_vars.message_enter_area), CUR_WIDTH - CUR_WIDTH / 3 - 50, 30);
    gtk_fixed_put(GTK_FIXED(chat_area), t_chat_room_vars.message_enter_area, L_FIELD_WIDTH, CUR_HEIGHT - 54);
    gtk_widget_set_name(GTK_WIDGET(t_chat_room_vars.message_enter_area), "message_enter_area"); 

    GtkWidget *add_image = gtk_event_box_new();
    gtk_widget_set_margin_top(GTK_WIDGET(add_image), 0);
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.message_enter_area), add_image, FALSE, FALSE, 8);
    gtk_widget_set_size_request(GTK_WIDGET(add_image), 33, 33);
    gtk_widget_set_name(GTK_WIDGET(add_image), "add_image");
    g_signal_connect(G_OBJECT(add_image), "button_press_event",
        G_CALLBACK(mx_attach), entry_chat);
    g_signal_connect(G_OBJECT(add_image), "enter-notify-event",
        G_CALLBACK(activate_prelight), &add_image);
    g_signal_connect(G_OBJECT(add_image), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);

    gtk_widget_set_name(GTK_WIDGET(entry_chat), "entry");
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.message_enter_area), entry_chat, TRUE, TRUE, 8);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_chat), text_for_labels[14]);
    g_signal_connect(G_OBJECT(entry_chat), "changed", 
        G_CALLBACK(entry_chat_fill_event), NULL);
    g_signal_connect(G_OBJECT(entry_chat), "activate", 
        G_CALLBACK(mx_send_message_on_enter), NULL);

    GtkWidget *ban_image = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.message_enter_area), ban_image, FALSE, FALSE, 8);
    mx_tooltip(text_for_labels[30], ban_image);
    gtk_widget_set_size_request(GTK_WIDGET(ban_image), 33, 33);
    gtk_widget_set_name(GTK_WIDGET(ban_image), "ban_image");

    GtkWidget *tick_image = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.message_enter_area), tick_image, FALSE, FALSE, 8);
    gtk_widget_set_size_request(GTK_WIDGET(tick_image), 33, 33);
    gtk_widget_set_name(GTK_WIDGET(tick_image), "tick_image");
    g_signal_connect(G_OBJECT(tick_image), "enter-notify-event",
        G_CALLBACK(activate_prelight), &tick_image);
    g_signal_connect(G_OBJECT(tick_image), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(tick_image), "button_press_event",
        G_CALLBACK(mx_send_message), entry_chat);

    GtkWidget *more_image = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(t_chat_room_vars.message_enter_area), more_image, FALSE, FALSE, 8);
    gtk_widget_set_size_request(GTK_WIDGET(more_image), 33, 33);
    gtk_widget_set_name(GTK_WIDGET(more_image), "more_image");
    g_signal_connect(G_OBJECT(more_image), "button_press_event",
        G_CALLBACK(mx_more_click), NULL);
    g_signal_connect(G_OBJECT(more_image), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(more_image), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);

    gtk_widget_show_all(GTK_WIDGET(t_chat_room_vars.message_enter_area));
    gtk_widget_hide(GTK_WIDGET(tick_image));
}
