#include "../../inc/uchat_client.h"

static void edit_username_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder);
static void edit_pseudonim_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder);

void mx_create_edit_user_form(void) {    
    // Create a blackout - a dark background behind the form
    //==================================================================================
    blackout = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(blackout), "blackout");
    gtk_fixed_put(GTK_FIXED(chat_area), blackout, 0, 0);
    gtk_widget_set_size_request(GTK_WIDGET(blackout), CUR_WIDTH, CUR_HEIGHT);
    //==================================================================================

    t_edit_user.NewFirstName = strdup(t_user.FirstName);
    if (t_user.SecondName != NULL)
        t_edit_user.NewSecondName = strdup(t_user.SecondName);
    else
        t_edit_user.NewSecondName = strdup("");
    t_edit_user.NewPseudonim = strdup(t_user.pseudonim);
    t_edit_user.NewDescription = strdup(t_user.description);
    t_edit_user.NewAvatar = gdk_pixbuf_copy(t_user.avatar);

    GtkBuilder *builder;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "client/data/edit_user.ui", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }
    
    GtkWidget *edit_user_form = GTK_WIDGET(gtk_builder_get_object(builder, "edit_user_form"));
    gtk_widget_set_name(GTK_WIDGET(edit_user_form), "edit_user_form");
    gtk_container_add(GTK_CONTAINER(blackout), edit_user_form);
    g_signal_connect(G_OBJECT(blackout), "button_press_event",
        G_CALLBACK(blackout_destroy), edit_user_form);

    GtkWidget *edit_user_main_screen = GTK_WIDGET(gtk_builder_get_object(builder, "edit_user_main_screen"));
    gtk_widget_set_size_request(GTK_WIDGET(edit_user_form), 400, 520);

    GtkWidget *edit_username_event_screen = GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_event_screen"));
    gtk_widget_set_size_request(GTK_WIDGET(edit_username_event_screen), 440, 500);
    mx_configure_username_event_screen(builder);

    GtkWidget *edit_pseudonim_event_screen = GTK_WIDGET(gtk_builder_get_object(builder, "edit_pseudonim_event_screen"));
    gtk_widget_set_size_request(GTK_WIDGET(edit_pseudonim_event_screen), 440, 500);
    mx_configure_pseudonim_event_screen(builder);


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
        G_CALLBACK(close_image_click_event), NULL);
    //==================================================================================

    // "change avatar" field
    //==================================================================================
    GtkWidget *avatar = GTK_WIDGET(gtk_builder_get_object(builder, "avatar"));
    gtk_widget_set_size_request(GTK_WIDGET(avatar), 100, 100);
    g_signal_connect(G_OBJECT(avatar), "draw", G_CALLBACK(mx_draw_event_image_avatar), &t_edit_user.NewAvatar);

    GtkWidget *change_avatar_btn = GTK_WIDGET(gtk_builder_get_object(builder, "change_avatar_btn"));
    gtk_button_set_label(GTK_BUTTON(change_avatar_btn), text_for_labels[5]);
    gtk_widget_set_name(GTK_WIDGET(change_avatar_btn), "edit_button");

    g_signal_connect(G_OBJECT(change_avatar_btn), "clicked",
        G_CALLBACK(change_avatart_btn_click), NULL);

    //==================================================================================

    // "change name" field
    //==================================================================================
    GtkWidget *edit_username_eventbox = GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_eventbox"));
    gtk_widget_set_name(GTK_WIDGET(edit_username_eventbox), "edit_eventbox");
    gtk_widget_set_size_request(GTK_WIDGET(edit_username_eventbox), 400, 40);

    GtkWidget *edit_username_box = GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_box"));
    gtk_widget_set_size_request(GTK_WIDGET(edit_username_box), 400, 0);

    GtkWidget *edit_username_icon = GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_icon"));
    gtk_widget_set_name(GTK_WIDGET(edit_username_icon), "edit_username_icon");
    gtk_widget_set_size_request(GTK_WIDGET(edit_username_icon), 30, 30);
    
    char *username_tmp = strdup(t_user.FirstName);
    username_tmp = mx_strjoin(username_tmp, " ");
    username_tmp = mx_strjoin(username_tmp, t_user.SecondName);
    GtkWidget *edit_username_label = GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_label"));
    gtk_label_set_text(GTK_LABEL(edit_username_label), username_tmp);
    free(username_tmp);
    gtk_widget_set_name(GTK_WIDGET(edit_username_label), "edit_label");

    GtkWidget *edit_username_pen = GTK_WIDGET(gtk_builder_get_object(builder, "edit_username_pen"));
    gtk_widget_set_name(GTK_WIDGET(edit_username_pen), "pen");
    gtk_widget_set_size_request(GTK_WIDGET(edit_username_pen), 20, 20);

    g_signal_connect(G_OBJECT(edit_username_eventbox), "enter-notify-event",
        G_CALLBACK(edit_user_eventbox_enter_notify), NULL);
    g_signal_connect(G_OBJECT(edit_username_eventbox), "leave-notify-event",
        G_CALLBACK(edit_user_eventbox_leave_notify), NULL);
    g_signal_connect(G_OBJECT(edit_username_eventbox), "button_press_event",
        G_CALLBACK(edit_username_click_event), (gpointer)builder);
    //==================================================================================

    // "change pseudo" field
    //==================================================================================
    GtkWidget *edit_pseudo_eventbox = GTK_WIDGET(gtk_builder_get_object(builder, "edit_pseudo_eventbox"));
    gtk_widget_set_name(GTK_WIDGET(edit_pseudo_eventbox), "edit_eventbox");
    gtk_widget_set_size_request(GTK_WIDGET(edit_pseudo_eventbox), 400, 40);

    GtkWidget *edit_pseudo_box = GTK_WIDGET(gtk_builder_get_object(builder, "edit_pseudo_box"));
    gtk_widget_set_size_request(GTK_WIDGET(edit_pseudo_box), 400, 0);
    GtkWidget *edit_pseudo_icon = GTK_WIDGET(gtk_builder_get_object(builder, "edit_pseudo_icon"));
    gtk_widget_set_name(GTK_WIDGET(edit_pseudo_icon), "edit_pseudo_icon");
    gtk_widget_set_size_request(GTK_WIDGET(edit_pseudo_icon), 30, 30);

    GtkWidget *edit_pseudo_label = GTK_WIDGET(gtk_builder_get_object(builder, "edit_pseudo_label"));
    gtk_label_set_text(GTK_LABEL(edit_pseudo_label), t_user.pseudonim);
    gtk_widget_set_name(GTK_WIDGET(edit_pseudo_label), "edit_label");

    GtkWidget *edit_pseudo_pen = GTK_WIDGET(gtk_builder_get_object(builder, "edit_pseudo_pen"));
    gtk_widget_set_name(GTK_WIDGET(edit_pseudo_pen), "pen");
    gtk_widget_set_size_request(GTK_WIDGET(edit_pseudo_pen), 20, 20);

    g_signal_connect(G_OBJECT(edit_pseudo_eventbox), "enter-notify-event",
        G_CALLBACK(edit_user_eventbox_enter_notify), NULL);
    g_signal_connect(G_OBJECT(edit_pseudo_eventbox), "leave-notify-event",
        G_CALLBACK(edit_user_eventbox_leave_notify), NULL);
    g_signal_connect(G_OBJECT(edit_pseudo_eventbox), "button_press_event",
        G_CALLBACK(edit_pseudonim_click_event), (gpointer)builder);
    //==================================================================================

    // "change description" field
    //==================================================================================
    GtkWidget *change_description_label = GTK_WIDGET(gtk_builder_get_object(builder, "change_description_label"));
    gtk_label_set_text(GTK_LABEL(change_description_label), text_for_labels[6]);
    gtk_widget_set_name(GTK_WIDGET(change_description_label), "edit_label");

    GtkWidget *change_description_box = GTK_WIDGET(gtk_builder_get_object(builder, "change_description_box"));
    gtk_widget_set_name(GTK_WIDGET(change_description_box), "change_description_box");
    
    GtkWidget *change_description_entry = GTK_WIDGET(gtk_builder_get_object(builder, "change_description_entry"));
    gtk_widget_set_name(GTK_WIDGET(change_description_entry), "change_description_entry");
    gtk_widget_set_size_request(GTK_WIDGET(change_description_entry), 400, 100);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(change_description_entry), GTK_WRAP_WORD);
    gtk_widget_set_state_flags(GTK_WIDGET(change_description_entry), GTK_STATE_FLAG_NORMAL, TRUE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(change_description_entry));
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), 
        t_user.description, strlen(t_user.description));

    g_signal_connect(G_OBJECT(change_description_entry), "button_press_event",
        G_CALLBACK(gtk_widget_grab_focus), NULL);

    g_signal_connect(G_OBJECT(buffer), "changed",
        G_CALLBACK(change_description_entry_change_event), NULL);
    //==================================================================================

    // Buttons to exit
    //==================================================================================
    GtkWidget *exit_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_margin_top(GTK_WIDGET(exit_buttons_box), 15);
    gtk_box_pack_start(GTK_BOX(edit_user_main_screen), exit_buttons_box, FALSE, FALSE, 15);

    GtkWidget *commit_btn = gtk_button_new_with_label(text_for_labels[12]);
    gtk_widget_set_name(GTK_WIDGET(commit_btn), "edit_button");
    gtk_button_set_relief(GTK_BUTTON(commit_btn), GTK_RELIEF_NONE);

    GtkWidget *exit_btn = gtk_button_new_with_label(text_for_labels[11]);
    gtk_widget_set_name(GTK_WIDGET(exit_btn), "edit_button");
    gtk_button_set_relief(GTK_BUTTON(exit_btn), GTK_RELIEF_NONE);

    gtk_box_pack_start(GTK_BOX(exit_buttons_box), exit_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(exit_buttons_box), commit_btn, TRUE, FALSE, 0);

    gtk_widget_set_size_request(GTK_WIDGET(commit_btn), 100, 0);
    gtk_widget_set_size_request(GTK_WIDGET(exit_btn), 100, 0);

    g_signal_connect(G_OBJECT(exit_btn), "button_press_event",
        G_CALLBACK(close_image_click_event), NULL);
    g_signal_connect(G_OBJECT(commit_btn), "button_press_event",
        G_CALLBACK(commit_edit_user_click_event), NULL);
    //==================================================================================

    gtk_widget_show_all(GTK_WIDGET(blackout));
    gtk_widget_hide(GTK_WIDGET(edit_username_event_screen));
    gtk_widget_hide(GTK_WIDGET(edit_pseudonim_event_screen));
    gtk_widget_set_can_focus(GTK_WIDGET(chat_area), TRUE);
    gtk_widget_grab_focus(GTK_WIDGET(chat_area));

    //g_object_unref(G_OBJECT(builder));
}

static void edit_username_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_user_main_screen")));
        gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_username_event_screen")));
    }
}

static void edit_pseudonim_click_event(GtkWidget *widget, GdkEventButton *event,
    gpointer builder) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_user_main_screen")));
        gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "edit_pseudonim_event_screen")));
    }
}
