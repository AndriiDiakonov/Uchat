#include "../../inc/uchat_client.h"

// Messages button
//==================================================================================
void messages_click(GtkWidget *widget, GdkEventButton *event) {
    mx_destroy_popups();

        
}
//==================================================================================

// Settings button
//==================================================================================
void settings_click(GtkWidget *widget, GdkEventButton *event) {
    mx_destroy_popups();
    if(counter % 2 != 0) {
        gtk_label_set_text(GTK_LABEL(labels_head->data), NULL);
        gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_CHECKED, FALSE);
        gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT);
        if (active_leftbar_container != NULL)
            gtk_widget_hide(GTK_WIDGET(active_leftbar_container));
        
        gtk_widget_set_name(GTK_WIDGET(settings_image.box), "settings_image_click");
        
        
        active_leftbar_container = settings_menu;
        // gtk_widget_hide(GTK_WIDGET(entry_search));
        gtk_widget_show(GTK_WIDGET(active_leftbar_container));
        counter++;
    } else {
        labels_head->index = 4;
        gtk_label_set_text(GTK_LABEL(labels_head->data), text_for_labels[labels_head->index]);
        gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_CHECKED, FALSE);
        gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT);
        if (active_leftbar_container != NULL)
            gtk_widget_hide(GTK_WIDGET(active_leftbar_container));

        gtk_widget_set_name(GTK_WIDGET(settings_image.box), "settings_image");

        // gtk_widget_show(GTK_WIDGET(entry_search));
        active_leftbar_container = chats_list;
        gtk_widget_show(GTK_WIDGET(active_leftbar_container));
        counter++;
    }
        
}
//==================================================================================
