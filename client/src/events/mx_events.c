#include "../../inc/uchat_client.h"

void activate_prelight(GtkWidget *widget) {
    GtkStateFlags flags = gtk_widget_get_state_flags(GTK_WIDGET(widget));
    if (!(flags & GTK_STATE_FLAG_CHECKED))
        gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT, FALSE);
}
void deactivate_prelight(GtkWidget *widget) {
    GtkStateFlags flags = gtk_widget_get_state_flags(GTK_WIDGET(widget));
    if (!(flags & GTK_STATE_FLAG_CHECKED))
        gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT);
}

void settings_element_enter_notify(GtkWidget *widget) {
    GList *parent_eventbox_children = gtk_container_get_children(GTK_CONTAINER(widget));
    GList *box_children = gtk_container_get_children(GTK_CONTAINER(parent_eventbox_children->data));
    g_list_free(parent_eventbox_children);

    gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT, TRUE);
    gtk_widget_set_state_flags(GTK_WIDGET(box_children->data), 
        GTK_STATE_FLAG_PRELIGHT, FALSE);

    g_list_free(box_children);
}

void settings_element_leave_notify(GtkWidget *widget) {
    GList *parent_eventbox_children = gtk_container_get_children(GTK_CONTAINER(widget));
    GList *box_children = gtk_container_get_children(GTK_CONTAINER(parent_eventbox_children->data));
    g_list_free(parent_eventbox_children);

    gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT);
    gtk_widget_unset_state_flags(GTK_WIDGET(box_children->data), GTK_STATE_FLAG_PRELIGHT);

    g_list_free(box_children);
}

// Blackout
//==============================================================
void blackout_destroy(GtkWidget *widget, GdkEventButton *event, GtkWidget *box) {
    if ( (event->type == GDK_BUTTON_PRESS && event->button == 1) ) {
        GtkAllocation alloc;
        gtk_widget_get_allocation(GTK_WIDGET(box), &alloc);
        if ( (event->x < alloc.x || event->x > (alloc.x + alloc.width))
            || (event->y < alloc.y || event->y > (alloc.y + alloc.height)) ) {
            gtk_widget_destroy(GTK_WIDGET(blackout));
            blackout = NULL;

            if (t_edit_user.NewFirstName != NULL) {
                free(t_edit_user.NewFirstName);
                t_edit_user.NewFirstName = NULL;
                free(t_edit_user.NewPseudonim);
                t_edit_user.NewPseudonim = NULL;
                free(t_edit_user.NewDescription);
                t_edit_user.NewDescription = NULL;
                g_object_unref(G_OBJECT(t_edit_user.NewAvatar));
                t_edit_user.NewAvatar = NULL;
            }
            if (t_edit_user.NewSecondName != NULL) {
                free(t_edit_user.NewSecondName);
                t_edit_user.NewSecondName = NULL;
            }
        }

        gtk_widget_set_can_focus(GTK_WIDGET(chat_area), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(chat_area));
    }
}
//==============================================================

// Close image
//========================================================
void close_image_click_event(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        if (t_edit_user.NewFirstName != NULL) {
            free(t_edit_user.NewFirstName);
            t_edit_user.NewFirstName = NULL;
            free(t_edit_user.NewPseudonim);
            t_edit_user.NewPseudonim = NULL;
            free(t_edit_user.NewDescription);
            t_edit_user.NewDescription = NULL;
            g_object_unref(G_OBJECT(t_edit_user.NewAvatar));
        }
        if (t_edit_user.NewSecondName != NULL)
            free(t_edit_user.NewSecondName);
        t_edit_user.NewSecondName = NULL;
        gtk_widget_destroy(GTK_WIDGET(blackout));
        blackout = NULL;

        gtk_widget_set_can_focus(GTK_WIDGET(chat_area), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(chat_area));
    }
}
//========================================================
