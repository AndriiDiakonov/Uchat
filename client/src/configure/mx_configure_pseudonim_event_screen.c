#include "../../inc/uchat_client.h"

void mx_configure_pseudonim_event_screen(GtkBuilder *builder) {
     
    // Return image
    //==================================================================================
    GtkWidget *return_image_box = GTK_WIDGET(gtk_builder_get_object(builder, "return_image_box1"));
    gtk_widget_set_name(GTK_WIDGET(return_image_box), "return_image_box");
    gtk_widget_set_size_request(GTK_WIDGET(return_image_box), 25, 25);

    g_signal_connect(G_OBJECT(return_image_box), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(return_image_box), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(return_image_box), "button_press_event",
        G_CALLBACK(return_pseudonim_click_event), (gpointer)builder);
    //==================================================================================

    // Draw avatar block
    //==================================================================================
    GtkWidget *pseudonim_avatar = GTK_WIDGET(gtk_builder_get_object(builder, "pseudonim_avatar"));
    gtk_widget_set_size_request(GTK_WIDGET(pseudonim_avatar), 100, 100);
    g_signal_connect(G_OBJECT(pseudonim_avatar), "draw", 
        G_CALLBACK(mx_draw_event_image_avatar), &t_edit_user.NewAvatar);
    //==================================================================================

    // "change pseudonim" block
    //==================================================================================
    GtkWidget *change_pseudo_label = GTK_WIDGET(gtk_builder_get_object(builder, "change_pseudo_label"));
    gtk_label_set_text(GTK_LABEL(change_pseudo_label), text_for_labels[9]);
    gtk_widget_set_name(GTK_WIDGET(change_pseudo_label), "edit_label");

    GtkWidget *change_pseudo_entry = GTK_WIDGET(gtk_builder_get_object(builder, "change_pseudo_entry"));
    gtk_widget_set_name(GTK_WIDGET(change_pseudo_entry), "edit_entry");
    gtk_entry_set_text(GTK_ENTRY(change_pseudo_entry), t_edit_user.NewPseudonim);
    gtk_widget_set_size_request(GTK_WIDGET(change_pseudo_entry), 400, 0);
    g_signal_connect(G_OBJECT(change_pseudo_entry), "changed",
        G_CALLBACK(pseudo_entry_changed_event), NULL);
    //==================================================================================

    // Additional info block
    //==================================================================================
    GtkWidget *info_label = GTK_WIDGET(gtk_builder_get_object(builder, "info_label"));
    gtk_label_set_text(GTK_LABEL(info_label), text_for_labels[10]);
    gtk_widget_set_name(GTK_WIDGET(info_label), "info_label");
    gtk_label_set_line_wrap(GTK_LABEL(info_label), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(info_label), 380, 0);
    //==================================================================================

    // Return buttons block
    //==================================================================================
    GtkWidget *return_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(builder, "edit_pseudonim_event_screen")), return_buttons_box, FALSE, FALSE, 0);
    gtk_widget_set_margin_top(GTK_WIDGET(return_buttons_box), 100);

    GtkWidget *commit_pseudonim_btn = gtk_button_new_with_label(text_for_labels[12]);
    gtk_widget_set_name(GTK_WIDGET(commit_pseudonim_btn), "edit_button");
    gtk_button_set_relief(GTK_BUTTON(commit_pseudonim_btn), GTK_RELIEF_NONE);

    GtkWidget *exit_pseudonim_btn = gtk_button_new_with_label(text_for_labels[15]);
    gtk_widget_set_name(GTK_WIDGET(exit_pseudonim_btn), "edit_button");
    gtk_button_set_relief(GTK_BUTTON(exit_pseudonim_btn), GTK_RELIEF_NONE);

    gtk_box_pack_start(GTK_BOX(return_buttons_box), exit_pseudonim_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(return_buttons_box), commit_pseudonim_btn, TRUE, FALSE, 0);

    gtk_widget_set_size_request(GTK_WIDGET(commit_pseudonim_btn), 100, 0);
    gtk_widget_set_size_request(GTK_WIDGET(exit_pseudonim_btn), 100, 0);

    g_signal_connect(G_OBJECT(exit_pseudonim_btn), "button_press_event",
        G_CALLBACK(return_pseudonim_click_event), (gpointer)builder);
    g_signal_connect(G_OBJECT(commit_pseudonim_btn), "button_press_event",
        G_CALLBACK(commit_pseudonim_click_event), (gpointer)builder);
    //==================================================================================
}
