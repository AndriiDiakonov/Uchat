#include "../../inc/uchat_client.h"

void return_username_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_event_screen")));
        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "change_fname_entry")), t_edit_user.NewFirstName);
        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "change_sname_entry")), t_edit_user.NewSecondName);
        gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "edit_user_main_screen")));
    }
}

void mx_configure_username_event_screen(GtkBuilder *builder) {
     
    // Return image
    //==================================================================================
    GtkWidget *return_image_box = GTK_WIDGET(gtk_builder_get_object(builder, "return_image_box"));
    gtk_widget_set_name(GTK_WIDGET(return_image_box), "return_image_box");
    gtk_widget_set_size_request(GTK_WIDGET(return_image_box), 25, 25);

    g_signal_connect(G_OBJECT(return_image_box), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(return_image_box), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(return_image_box), "button_press_event",
        G_CALLBACK(return_username_click_event), builder);
    //==================================================================================

    // Draw avatar block
    //==================================================================================
    GtkWidget *username_avatar = GTK_WIDGET(gtk_builder_get_object(builder, "username_avatar"));
    gtk_widget_set_size_request(GTK_WIDGET(username_avatar), 100, 100);
    g_signal_connect(G_OBJECT(username_avatar), "draw", 
        G_CALLBACK(mx_draw_event_image_avatar), &t_edit_user.NewAvatar);
    //==================================================================================

    // "change first name" block
    //==================================================================================
    GtkWidget *change_fname_label = GTK_WIDGET(gtk_builder_get_object(builder, "change_fname_label"));
    gtk_label_set_text(GTK_LABEL(change_fname_label), text_for_labels[7]);
    gtk_widget_set_name(GTK_WIDGET(change_fname_label), "edit_label");

    GtkWidget *change_fname_entry = GTK_WIDGET(gtk_builder_get_object(builder, "change_fname_entry"));
    gtk_widget_set_name(GTK_WIDGET(change_fname_entry), "edit_entry");
    gtk_entry_set_text(GTK_ENTRY(change_fname_entry), t_edit_user.NewFirstName);
    gtk_widget_set_size_request(GTK_WIDGET(change_fname_entry), 400, 0);
    g_signal_connect(G_OBJECT(change_fname_entry), "changed",
        G_CALLBACK(fname_entry_changed_event), NULL);
    //==================================================================================

    // "change second name" block
    //==================================================================================
    GtkWidget *change_sname_label = GTK_WIDGET(gtk_builder_get_object(builder, "change_sname_label"));
    gtk_label_set_text(GTK_LABEL(change_sname_label), text_for_labels[8]);
    gtk_widget_set_name(GTK_WIDGET(change_sname_label), "edit_label");

    GtkWidget *change_sname_entry = GTK_WIDGET(gtk_builder_get_object(builder, "change_sname_entry"));
    gtk_widget_set_name(GTK_WIDGET(change_sname_entry), "edit_entry");
    gtk_entry_set_text(GTK_ENTRY(change_sname_entry), t_edit_user.NewSecondName);
    gtk_widget_set_size_request(GTK_WIDGET(change_sname_entry), 400, 0);
    //==================================================================================

    // Return buttons block
    //==================================================================================
    GtkWidget *return_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(GTK_BUILDER(builder), "edit_username_event_screen")), return_buttons_box, FALSE, FALSE, 0);
    gtk_widget_set_margin_top(GTK_WIDGET(return_buttons_box), 100);

    GtkWidget *commit_username_btn = gtk_button_new_with_label(text_for_labels[12]);
    gtk_widget_set_name(GTK_WIDGET(commit_username_btn), "edit_button");
    gtk_button_set_relief(GTK_BUTTON(commit_username_btn), GTK_RELIEF_NONE);

    GtkWidget *exit_username_btn = gtk_button_new_with_label(text_for_labels[15]);
    gtk_widget_set_name(GTK_WIDGET(exit_username_btn), "edit_button");
    gtk_button_set_relief(GTK_BUTTON(exit_username_btn), GTK_RELIEF_NONE);

    gtk_box_pack_start(GTK_BOX(return_buttons_box), exit_username_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(return_buttons_box), commit_username_btn, TRUE, FALSE, 0);

    gtk_widget_set_size_request(GTK_WIDGET(commit_username_btn), 100, 0);
    gtk_widget_set_size_request(GTK_WIDGET(exit_username_btn), 100, 0);

    g_signal_connect(G_OBJECT(exit_username_btn), "button_press_event",
        G_CALLBACK(return_username_click_event), (gpointer)builder);
    g_signal_connect(G_OBJECT(commit_username_btn), "button_press_event",
        G_CALLBACK(commit_username_click_event), (gpointer)builder);
    //==================================================================================
}
