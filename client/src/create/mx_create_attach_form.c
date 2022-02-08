#include "../../inc/uchat_client.h"

void mx_create_attach_form(GtkWidget *entry, char *filename) {
    // Blackout with main container
    //==================================================================================
    blackout = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(blackout), "blackout");
    gtk_fixed_put(GTK_FIXED(chat_area), blackout, 0, 0);
    gtk_widget_set_size_request(GTK_WIDGET(blackout), CUR_WIDTH, CUR_HEIGHT);

    GtkWidget *attach_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(blackout), attach_container);
    gtk_widget_set_valign(GTK_WIDGET(attach_container), GTK_ALIGN_CENTER);
    gtk_widget_set_halign(GTK_WIDGET(attach_container), GTK_ALIGN_CENTER);

    g_signal_connect(G_OBJECT(blackout), "button_press_event",
        G_CALLBACK(blackout_destroy), attach_container);
    //==================================================================================

    // Image preview
    //==================================================================================
    GdkPixbuf *pixbuf = mx_create_pixbuf(filename);

    GdkPixbuf *prev_pixbuf = gdk_pixbuf_copy(pixbuf);
    prev_pixbuf = mx_size_image_down(prev_pixbuf, 500, 350);

    GtkWidget *image = gtk_drawing_area_new();
    gtk_widget_set_halign(GTK_WIDGET(image), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(image), gdk_pixbuf_get_width(GDK_PIXBUF(prev_pixbuf)), 
        gdk_pixbuf_get_height(GDK_PIXBUF(prev_pixbuf)));
    g_signal_connect(G_OBJECT(image), "draw", G_CALLBACK(draw_image), prev_pixbuf);

    gtk_box_pack_start(GTK_BOX(attach_container), image, TRUE, TRUE, 0);
    //==================================================================================

    // Entry for a message
    //==================================================================================
    GtkWidget *entry_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_top(GTK_WIDGET(entry_container), 20);
    gtk_widget_set_size_request(GTK_WIDGET(entry_container), 310, 40);
    gtk_box_pack_start(GTK_BOX(attach_container), entry_container, TRUE, TRUE, 0);

    GtkWidget *message_entry = gtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(message_entry), "attach_entry");
    gtk_widget_set_size_request(GTK_WIDGET(message_entry), 300, 0);
    gtk_widget_set_halign(GTK_WIDGET(attach_container), GTK_ALIGN_CENTER);
    gtk_entry_set_placeholder_text(GTK_ENTRY(message_entry), text_for_labels[14]);
    gtk_entry_set_text(GTK_ENTRY(message_entry), gtk_entry_get_text(GTK_ENTRY(entry)));
    gtk_box_pack_start(GTK_BOX(entry_container), message_entry, TRUE, TRUE, 0);

    void **arr = g_new(void *, 3);
    arr[0] = filename;
    arr[1] = pixbuf;
    arr[2] = prev_pixbuf;
    g_signal_connect(G_OBJECT(message_entry), "activate", 
        G_CALLBACK(mx_attach_send_message_on_enter), arr);
    g_signal_connect(G_OBJECT(message_entry), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(message_entry), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    //==================================================================================

    gtk_widget_show_all(GTK_WIDGET(blackout));
}
