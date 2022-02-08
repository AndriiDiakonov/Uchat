#include "../../inc/uchat_client.h"

static void edit_user_click(GtkWidget *widget, GdkEventButton *event);
static void change_account_click(GtkWidget *widget, GdkEventButton *event);
static void chat_settings_click(GtkWidget *widget, GdkEventButton *event);
static void language_click(GtkWidget *widget, GdkEventButton *event);

void mx_configure_settings_menu_area(void) {
    settings_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(GTK_WIDGET(settings_menu), L_FIELD_WIDTH, CUR_HEIGHT - 105);
    gtk_widget_set_name(GTK_WIDGET(settings_menu), "settings_menu");

    gtk_fixed_put(GTK_FIXED(chat_area), settings_menu, 0, 105);

    // Profile info section
    //==============================================================================================
    GtkWidget *user_profile_preview_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(user_profile_preview_eventbox), "user_profile_preview_eventbox");
    GtkWidget *user_profile_preview_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(user_profile_preview_box), "user_profile_preview_box");
    gtk_container_add(GTK_CONTAINER(user_profile_preview_eventbox), user_profile_preview_box);
    gtk_box_pack_start(GTK_BOX(settings_menu), user_profile_preview_eventbox, FALSE, FALSE, 0);
    
    GtkWidget *avatar = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(avatar), 70, 70);
    g_signal_connect(G_OBJECT(avatar), "draw", G_CALLBACK(mx_draw_event_image_avatar), &t_user.avatar);
    gtk_box_pack_start(GTK_BOX(user_profile_preview_box), avatar, FALSE, FALSE, 0);
    GtkWidget *user_info_preview = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(user_profile_preview_box), user_info_preview, FALSE, FALSE, 31);
    
    char *username_tmp = strdup(t_user.FirstName);
    username_tmp = mx_strjoin(username_tmp, " ");
    username_tmp = mx_strjoin(username_tmp, t_user.SecondName);
    t_edit_user.username = gtk_label_new(username_tmp);
    free(username_tmp);
    gtk_widget_set_name(GTK_WIDGET(t_edit_user.username), "username_preview");

    char *tmp_pseudonim = "@";
    tmp_pseudonim = mx_strjoin(tmp_pseudonim, t_user.pseudonim);
    t_edit_user.user_pseudonim = gtk_label_new(tmp_pseudonim);
    free(tmp_pseudonim);
    gtk_widget_set_name(GTK_WIDGET(t_edit_user.user_pseudonim), "user_pseudonim_preview");
    gtk_box_pack_start(GTK_BOX(user_info_preview), t_edit_user.username, TRUE, FALSE, 0);
    gtk_widget_set_halign(t_edit_user.username, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(user_info_preview), t_edit_user.user_pseudonim, FALSE, FALSE, 0);
    gtk_widget_set_halign(t_edit_user.user_pseudonim, GTK_ALIGN_START);
    //==============================================================================================

    // "Edit profile" section
    //==============================================================================================
    GtkWidget *edit_user_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(edit_user_eventbox), "setting_menu_eventbox");
    gtk_widget_set_size_request(GTK_WIDGET(edit_user_eventbox), L_FIELD_WIDTH, 50);
    GtkWidget *edit_user_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(edit_user_box), "change_box");
    gtk_container_add(GTK_CONTAINER(edit_user_eventbox), edit_user_box);
    gtk_box_pack_start(GTK_BOX(settings_menu), edit_user_eventbox, FALSE, FALSE, 0);

    GtkWidget *edit_user_image = gtk_event_box_new();
    gtk_widget_set_valign(GTK_WIDGET(edit_user_image), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(edit_user_image), 30, 30);
    gtk_widget_set_name(GTK_WIDGET(edit_user_image), "edit_user_image");
    gtk_box_pack_start(GTK_BOX(edit_user_box), edit_user_image, FALSE, FALSE, 0);

    GtkWidget *label_edit_user = gtk_label_new(text_for_labels[3]);
    gtk_label_set_max_width_chars(GTK_LABEL(label_edit_user), 16);
    gtk_widget_set_margin_start(GTK_WIDGET(label_edit_user), 50);
    mx_label_push_back(&labels_head, label_edit_user, 3);
    gtk_widget_set_name(GTK_WIDGET(label_edit_user), "label_settings_menu");
    gtk_box_pack_start(GTK_BOX(edit_user_box), label_edit_user, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(edit_user_eventbox), "enter-notify-event",
        G_CALLBACK(settings_element_enter_notify), NULL);
    g_signal_connect(G_OBJECT(edit_user_eventbox), "leave-notify-event",
        G_CALLBACK(settings_element_leave_notify), NULL);
    g_signal_connect(G_OBJECT(edit_user_eventbox), "button_press_event",
        G_CALLBACK(edit_user_click), NULL);   
    //==============================================================================================

    // "Change account" section
    //==============================================================================================
    GtkWidget *change_account_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(change_account_eventbox), "setting_menu_eventbox");
    gtk_widget_set_size_request(GTK_WIDGET(change_account_eventbox), L_FIELD_WIDTH, 50);
    GtkWidget *change_account_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(change_account_box), "change_box");
    gtk_container_add(GTK_CONTAINER(change_account_eventbox), change_account_box);
    gtk_box_pack_start(GTK_BOX(settings_menu), change_account_eventbox, FALSE, FALSE, 0);

    GtkWidget *change_account_image = gtk_event_box_new();
    gtk_widget_set_valign(GTK_WIDGET(change_account_image), GTK_ALIGN_END);
    gtk_widget_set_size_request(GTK_WIDGET(change_account_image), 30, 30);
    gtk_widget_set_name(GTK_WIDGET(change_account_image), "change_account_image");
    gtk_box_pack_start(GTK_BOX(change_account_box), change_account_image, FALSE, FALSE, 0);

    GtkWidget *label_change_account = gtk_label_new(text_for_labels[2]);
    gtk_label_set_max_width_chars(GTK_LABEL(label_change_account), 16);
    gtk_widget_set_margin_start(GTK_WIDGET(label_change_account), 50);
    mx_label_push_back(&labels_head, label_change_account, 2);
    gtk_widget_set_name(GTK_WIDGET(label_change_account), "label_settings_menu");
    gtk_box_pack_start(GTK_BOX(change_account_box), label_change_account, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(change_account_eventbox), "enter-notify-event",
        G_CALLBACK(settings_element_enter_notify), NULL);
    g_signal_connect(G_OBJECT(change_account_eventbox), "leave-notify-event",
        G_CALLBACK(settings_element_leave_notify), NULL);
    g_signal_connect(G_OBJECT(change_account_eventbox), "button_press_event",
        G_CALLBACK(change_account_click), NULL); 
    //==============================================================================================

    // "Chat settings" section
    //==============================================================================================
    GtkWidget *chat_settings_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(chat_settings_eventbox), "setting_menu_eventbox");
    gtk_widget_set_size_request(GTK_WIDGET(chat_settings_eventbox), L_FIELD_WIDTH, 50);
    GtkWidget *chat_settings_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(chat_settings_box), "change_box");
    gtk_container_add(GTK_CONTAINER(chat_settings_eventbox), chat_settings_box);
    gtk_box_pack_start(GTK_BOX(settings_menu), chat_settings_eventbox, FALSE, FALSE, 0);

    GtkWidget *chat_settings_image = gtk_event_box_new();
    gtk_widget_set_valign(GTK_WIDGET(chat_settings_image), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(chat_settings_image), 30, 30);
    gtk_widget_set_name(GTK_WIDGET(chat_settings_image), "chat_settings_image");
    gtk_box_pack_start(GTK_BOX(chat_settings_box), chat_settings_image, FALSE, FALSE, 0);

    GtkWidget *label_chat_settings = gtk_label_new(text_for_labels[1]);
    gtk_label_set_max_width_chars(GTK_LABEL(label_chat_settings), 16);
    gtk_widget_set_margin_start(GTK_WIDGET(label_chat_settings), 50);
    mx_label_push_back(&labels_head, label_chat_settings, 1);
    gtk_widget_set_name(GTK_WIDGET(label_chat_settings), "label_settings_menu");
    gtk_box_pack_start(GTK_BOX(chat_settings_box), label_chat_settings, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(chat_settings_eventbox), "enter-notify-event",
        G_CALLBACK(settings_element_enter_notify), NULL);
    g_signal_connect(G_OBJECT(chat_settings_eventbox), "leave-notify-event",
        G_CALLBACK(settings_element_leave_notify), NULL);
    g_signal_connect(G_OBJECT(chat_settings_eventbox), "button_press_event",
        G_CALLBACK(chat_settings_click), NULL); 
    //==============================================================================================

    // "Language" section
    //==============================================================================================
    GtkWidget *language_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(language_eventbox), "setting_menu_eventbox");
    gtk_widget_set_size_request(GTK_WIDGET(language_eventbox), L_FIELD_WIDTH, 50);
    GtkWidget *language_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(language_box), "language_box");
    gtk_container_add(GTK_CONTAINER(language_eventbox), language_box);
    gtk_box_pack_start(GTK_BOX(settings_menu), language_eventbox, FALSE, FALSE, 0);

    GtkWidget *language_image = gtk_event_box_new();
    gtk_widget_set_valign(GTK_WIDGET(language_image), GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(GTK_WIDGET(language_image), 30, 30);
    gtk_widget_set_name(GTK_WIDGET(language_image), "language_image");
    gtk_box_pack_start(GTK_BOX(language_box), language_image, FALSE, FALSE, 0);

    GtkWidget *label_language = gtk_label_new(text_for_labels[0]);
    gtk_label_set_max_width_chars(GTK_LABEL(label_language), 16);
    gtk_widget_set_margin_start(GTK_WIDGET(label_language), 50);
    mx_label_push_back(&labels_head, label_language, 0);
    gtk_widget_set_name(GTK_WIDGET(label_language), "label_settings_menu");
    gtk_box_pack_start(GTK_BOX(language_box), label_language, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(language_eventbox), "enter-notify-event",
        G_CALLBACK(settings_element_enter_notify), NULL);
    g_signal_connect(G_OBJECT(language_eventbox), "leave-notify-event",
        G_CALLBACK(settings_element_leave_notify), NULL);
    g_signal_connect(G_OBJECT(language_eventbox), "button_press_event",
        G_CALLBACK(language_click), NULL);
    //==============================================================================================

    // Footer section
    //==============================================================================================
    GtkWidget *label_postman = gtk_label_new("Postman");
    gtk_widget_set_name(GTK_WIDGET(label_postman), "label_postman");
    GtkWidget *label_version = gtk_label_new("Beta version");
    gtk_widget_set_name(GTK_WIDGET(label_version), "label_version");
    gtk_box_pack_start(GTK_BOX(settings_menu), label_postman, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(settings_menu), label_version, TRUE, TRUE, 0);

    gtk_widget_set_valign(GTK_WIDGET(label_postman), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(label_version), GTK_ALIGN_END);
    //============================================================================================== 

    active_leftbar_container = settings_menu;
}

// Edit_user icon
//=================================================================================
static void edit_user_click(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        mx_create_edit_user_form();
    }
}
//=================================================================================

// Change_account icon
//=================================================================================
static void change_account_click(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        if (t_chat_room_vars.message_enter_area != NULL) {
            gtk_widget_destroy(GTK_WIDGET(t_chat_room_vars.message_enter_area));
            t_chat_room_vars.message_enter_area = NULL;
            gtk_widget_destroy(GTK_WIDGET(t_chat_room_vars.right_container));
            t_chat_room_vars.right_container = NULL;
        }
        if (curr_room_msg_head != NULL)
            mx_clear_message_list(&curr_room_msg_head);
        mx_create_registration_menu();
    }
}
//=================================================================================

// Chat_settings icon
//=================================================================================
static void chat_settings_click(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        mx_create_theme_menu();        
    }
}
//=================================================================================

// Language icon
//=================================================================================
static void language_click(GtkWidget *widget, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        mx_create_language_menu();
    }
}
//=================================================================================
