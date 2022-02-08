#include "../../inc/uchat_client.h"

void edit_user_eventbox_enter_notify(GtkWidget *widget) {
    gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT, TRUE);

    GList *parent_eventbox_children = gtk_container_get_children(GTK_CONTAINER(widget));
    GList *box_children = gtk_container_get_children(GTK_CONTAINER(parent_eventbox_children->data));
    g_list_free(parent_eventbox_children);
    gtk_widget_set_state_flags(GTK_WIDGET(box_children->data), GTK_STATE_FLAG_PRELIGHT, TRUE);
    gtk_widget_set_state_flags(GTK_WIDGET(g_list_last(box_children)->data), GTK_STATE_FLAG_PRELIGHT, TRUE);
    g_list_free(box_children);
}

void edit_user_eventbox_leave_notify(GtkWidget *widget) {
    gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_PRELIGHT);

    GList *parent_eventbox_children = gtk_container_get_children(GTK_CONTAINER(widget));
    GList *box_children = gtk_container_get_children(GTK_CONTAINER(parent_eventbox_children->data));
    g_list_free(parent_eventbox_children);
    gtk_widget_unset_state_flags(GTK_WIDGET(box_children->data), GTK_STATE_FLAG_PRELIGHT);
    gtk_widget_unset_state_flags(GTK_WIDGET(g_list_last(box_children)->data), GTK_STATE_FLAG_PRELIGHT);
    g_list_free(box_children);
}

// Change avatar button
//============================================================================================
void change_avatart_btn_click(GtkWidget *widget, GdkEvent *event) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Select an image",
                                        GTK_WINDOW(window),
                                        action,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Select",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        char *filename = gtk_file_chooser_get_filename (chooser);
        g_object_unref(G_OBJECT(t_edit_user.NewAvatar));
        t_edit_user.NewAvatar = mx_get_pixbuf_with_size(filename, 100, 100);
        mx_write_photo_to_bd(filename, t_user.id);
    }

    gtk_widget_destroy (dialog);
}
//============================================================================================

// Edit username field
//============================================================================================
void fname_entry_changed_event(GtkWidget *widget) {
    if (strlen(gtk_entry_get_text(GTK_ENTRY(widget))) == 0) {
        gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_LINK, TRUE);
    }
    else {
        gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_LINK);
    }
}

void commit_username_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1
        && strlen(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_fname_entry")))) != 0) {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_username_event_screen")));

        free(t_edit_user.NewFirstName);
        t_edit_user.NewFirstName = NULL;
        t_edit_user.NewFirstName = mx_strjoin(t_edit_user.NewFirstName, 
            gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_fname_entry"))));

        if (t_edit_user.NewSecondName != NULL) {
            free(t_edit_user.NewSecondName);
            t_edit_user.NewSecondName = strdup("");
        }
        if (strlen(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_sname_entry")))) != 0)
            t_edit_user.NewSecondName = mx_strjoin(t_edit_user.NewSecondName, 
                gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_sname_entry"))));

        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_fname_entry")), t_edit_user.NewFirstName);
        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_sname_entry")), t_edit_user.NewSecondName);
        

        char *username_tmp = strdup(t_edit_user.NewFirstName);
        username_tmp = mx_strjoin(username_tmp, " ");
        username_tmp = mx_strjoin(username_tmp, t_edit_user.NewSecondName);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(builder), "edit_username_label")), username_tmp);
        free(username_tmp);

        gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_user_main_screen")));
    }
}

//============================================================================================

// Edit pseudonim field
//============================================================================================
void pseudo_entry_changed_event(GtkWidget *widget) {
    if (strlen(gtk_entry_get_text(GTK_ENTRY(widget))) <= 5) {
        gtk_widget_set_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_LINK, TRUE);
    }
    else {
        gtk_widget_unset_state_flags(GTK_WIDGET(widget), GTK_STATE_FLAG_LINK);
    }
}

void return_pseudonim_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_pseudonim_event_screen")));
        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_pseudo_entry")), t_edit_user.NewPseudonim);
        gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_user_main_screen")));
    }
}

void commit_pseudonim_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1 
        && strlen(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_pseudo_entry")))) > 5) {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_pseudonim_event_screen")));
        if (t_edit_user.NewPseudonim == NULL)
            t_edit_user.NewPseudonim = mx_strjoin(t_edit_user.NewPseudonim, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_pseudo_entry"))));
        else {
            free(t_edit_user.NewPseudonim);
            t_edit_user.NewPseudonim = NULL;
            t_edit_user.NewPseudonim = mx_strjoin(t_edit_user.NewPseudonim, gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_pseudo_entry"))));
        }
        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(builder), "change_pseudo_entry")), t_edit_user.NewPseudonim);

        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(builder), "edit_pseudo_label")), t_edit_user.NewPseudonim);
        gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_user_main_screen")));
    }
}
//============================================================================================

// Change description field
//============================================================================================
void change_description_entry_change_event(GtkWidget *widget) {
    if (t_edit_user.NewDescription != NULL) {
        free(t_edit_user.NewDescription);
        t_edit_user.NewDescription = NULL;
    }
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER(widget), &start);
    gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER(widget), &end);
    t_edit_user.NewDescription = mx_strjoin(t_edit_user.NewDescription, gtk_text_buffer_get_text(GTK_TEXT_BUFFER(widget), &start, &end, FALSE));
}
//============================================================================================

// "Apply button" in the form
//============================================================================================
void commit_edit_user_click_event(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        mx_change_user_name(t_edit_user.NewFirstName, t_edit_user.NewSecondName);
        free(t_edit_user.NewFirstName);
        t_edit_user.NewFirstName = NULL;
        if (t_edit_user.NewSecondName != NULL) {
            free(t_edit_user.NewSecondName);
            t_edit_user.NewSecondName = NULL;
        }

        char *username_tmp = strdup(t_user.FirstName);
        username_tmp = mx_strjoin(username_tmp, " ");
        username_tmp = mx_strjoin(username_tmp, t_user.SecondName);
        gtk_label_set_text(GTK_LABEL(t_edit_user.username), username_tmp);
        free(username_tmp);


        mx_change_user_pseudonim(t_edit_user.NewPseudonim);
        free(t_edit_user.NewPseudonim);
        t_edit_user.NewPseudonim = NULL;

        char *tmp_text = "@";
        tmp_text = mx_strjoin(tmp_text, t_user.pseudonim);
        gtk_label_set_text(GTK_LABEL(t_edit_user.user_pseudonim), tmp_text);
        free(tmp_text);

        mx_change_user_description(t_edit_user.NewDescription);
        free(t_edit_user.NewDescription);
        t_edit_user.NewDescription = NULL;
        mx_edit_name(t_user.FirstName, t_user.SecondName, t_user.pseudonim, t_user.description, t_user.id);

        g_object_unref(G_OBJECT(t_user.avatar));
        t_user.avatar = gdk_pixbuf_copy(GDK_PIXBUF(t_edit_user.NewAvatar));
        g_object_unref(G_OBJECT(t_edit_user.NewAvatar));
        t_edit_user.NewAvatar = NULL;
        gtk_widget_queue_draw(GTK_WIDGET(settings_menu));

        gtk_widget_destroy(GTK_WIDGET(blackout));
        blackout = NULL;
    }
}
//============================================================================================
