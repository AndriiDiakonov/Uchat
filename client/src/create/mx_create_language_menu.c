#include "../../inc/uchat_client.h"

static void mx_language_close(GtkWidget *widget, GdkEventButton *event);

void mx_create_language_menu(void) {
    // Create a blackout - a dark background behind the form
    //==================================================================================
    blackout = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(blackout), "blackout");    
    gtk_fixed_put(GTK_FIXED(chat_area), blackout, 0, 0);
    gtk_widget_set_size_request(GTK_WIDGET(blackout), CUR_WIDTH, CUR_HEIGHT);
    //==================================================================================

    GtkBuilder *builder;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "client/data/language.ui", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    GtkWidget *language_form = GTK_WIDGET(gtk_builder_get_object(builder, "language_form"));
    gtk_container_add(GTK_CONTAINER(blackout), language_form);
    gtk_widget_set_name(GTK_WIDGET(language_form), "edit_user_form");
    //gtk_container_add(GTK_CONTAINER(blackout), language_form);
    g_signal_connect(G_OBJECT(blackout), "button_press_event",
        G_CALLBACK(blackout_destroy), language_form);
    

    // "close" image
    //==================================================================================
    GtkWidget *close_image_box = GTK_WIDGET(gtk_builder_get_object(builder, "close_image_box"));
    gtk_widget_set_name(GTK_WIDGET(close_image_box), "close_image_box");
    gtk_widget_set_size_request(GTK_WIDGET(close_image_box), 25, 25);

    g_signal_connect(G_OBJECT(close_image_box), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(close_image_box), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(close_image_box), "button_press_event",
        G_CALLBACK(mx_language_close), NULL);
    //==================================================================================

    
    // Create and pack widgets
    gtk_box_pack_start(GTK_BOX(language_form),
        mx_language_create_box("client/img/USA.png", "English"),
        FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(language_form),
        mx_language_create_box("client/img/russia.png", "Русский"),
        FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(language_form),
        mx_language_create_box("client/img/ukraine.png", "Українська"),
        FALSE, FALSE, 0);

    gtk_widget_show_all(GTK_WIDGET(blackout));

    g_object_unref(G_OBJECT(builder));
}

static void mx_language_close(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_widget_destroy(GTK_WIDGET(blackout));
        blackout = NULL;
    }
}
