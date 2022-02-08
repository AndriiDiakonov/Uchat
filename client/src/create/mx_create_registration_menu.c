#include "../../inc/uchat_client.h"

static void login_activate(GtkWidget *widget) {
    gtk_widget_set_can_focus(GTK_WIDGET(t_authorization.password), TRUE);
    gtk_widget_grab_focus(GTK_WIDGET(t_authorization.password));
}

static void drag_a_focus(GtkWidget *widget, GtkWidget *aim);

void mx_create_registration_menu(void) {
    t_authorization.fail_inscription = gtk_label_new(text_for_labels[31]);
    t_authorization.fail_auto_inscription = gtk_label_new(text_for_labels[32]);

    // Background
    //=================================================================================
    t_authorization.authorization_container = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(t_authorization.authorization_container), "authorization_container");
    gtk_fixed_put(GTK_FIXED(authorization_area), t_authorization.authorization_container, 0, 0);
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.authorization_container), CUR_WIDTH, CUR_HEIGHT);
    //=================================================================================


    // Authorization menu
    //=================================================================================
    GtkWidget *main_authorization_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(GTK_WIDGET(main_authorization_menu), 110, 110);
    gtk_widget_set_name(GTK_WIDGET(main_authorization_menu), "registration_menu_form");
    gtk_widget_set_valign(GTK_WIDGET(main_authorization_menu), GTK_ALIGN_CENTER);
    gtk_widget_set_halign(GTK_WIDGET(main_authorization_menu), GTK_ALIGN_START);
    gtk_container_add(GTK_CONTAINER(t_authorization.authorization_container), main_authorization_menu);
    //=================================================================================

    // Close button
    //=================================================================================
    if(t_user.id != -1) {
        GtkWidget *close_image_box = gtk_event_box_new();
        gtk_widget_set_name(GTK_WIDGET(close_image_box), "close_image_box");
        gtk_widget_set_size_request(GTK_WIDGET(close_image_box), 25, 25);
        gtk_box_pack_start(GTK_BOX(main_authorization_menu), close_image_box, FALSE, FALSE, 0);
        gtk_widget_set_halign(GTK_WIDGET(close_image_box), GTK_ALIGN_START);

        g_signal_connect(G_OBJECT(close_image_box), "enter-notify-event",
            G_CALLBACK(activate_prelight), NULL);
        g_signal_connect(G_OBJECT(close_image_box), "leave-notify-event",
            G_CALLBACK(deactivate_prelight), NULL);
        g_signal_connect(G_OBJECT(close_image_box), "button_press_event",
            G_CALLBACK(authorization_close), NULL);
    }
    //=================================================================================

    // Log in menu
    //=================================================================================
    t_authorization.log_in_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_authorization_menu), t_authorization.log_in_menu, FALSE, FALSE, 0);

    // Inscription Log in
    GtkWidget *log_in_inscription = gtk_label_new(text_for_labels[16]);
    gtk_box_pack_start(GTK_BOX(t_authorization.log_in_menu), log_in_inscription, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(log_in_inscription), "authorization");

    // Entry for login
    t_authorization.login = gtk_entry_new();
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.login), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.login), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.login), "entry_data");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.login), 400, 50);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.login), text_for_labels[17]);
    gtk_box_pack_start(GTK_BOX(t_authorization.log_in_menu), t_authorization.login, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(t_authorization.login), "changed",
        G_CALLBACK(button_shine), NULL);

    // Entry for password
    GtkWidget *password_eye_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(password_eye_box), "password_eye_back");
    gtk_widget_set_margin_start(GTK_WIDGET(password_eye_box), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(password_eye_box), 20);
    gtk_box_pack_start(GTK_BOX(t_authorization.log_in_menu), password_eye_box, FALSE, FALSE, 30);

    t_authorization.password = gtk_entry_new();
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.password), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.password), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.password), "authorization_pass_entry");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.password), 335, 0);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.password), text_for_labels[18]);
    gtk_box_pack_start(GTK_BOX(password_eye_box), t_authorization.password, FALSE, FALSE, 0);
    gtk_entry_set_visibility(GTK_ENTRY(t_authorization.password), FALSE);
    g_signal_connect(G_OBJECT(t_authorization.password), "changed",
        G_CALLBACK(button_shine), NULL);
    g_signal_connect(G_OBJECT(t_authorization.login), "activate",
        G_CALLBACK(drag_a_focus), t_authorization.password);
    g_signal_connect(G_OBJECT(t_authorization.password), "activate",
        G_CALLBACK(authorization), t_authorization.fail_auto_inscription);

    GtkWidget *eye_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(eye_eventbox), "eye_password");
    gtk_box_pack_start(GTK_BOX(password_eye_box), eye_eventbox, FALSE, FALSE, 5);
    gtk_widget_set_size_request(GTK_WIDGET(eye_eventbox), 35, 30);
    gtk_widget_set_valign(GTK_WIDGET(eye_eventbox), GTK_ALIGN_END);
    g_signal_connect(G_OBJECT(eye_eventbox), "button_press_event",
        G_CALLBACK(eye_pressed), t_authorization.password);

    // Inscription fail
    gtk_box_pack_start(GTK_BOX(t_authorization.log_in_menu), t_authorization.fail_auto_inscription, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.fail_auto_inscription), "error");
    
    // Log in button
    t_authorization.login_btn = gtk_event_box_new();
    GtkWidget *login_btn_label = gtk_label_new(text_for_labels[19]);
    gtk_container_add(GTK_CONTAINER(t_authorization.login_btn), login_btn_label);
    gtk_box_pack_start(GTK_BOX(t_authorization.log_in_menu), t_authorization.login_btn, FALSE, FALSE, 10);
    gtk_widget_set_margin_top(GTK_WIDGET(t_authorization.login_btn), 30);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.login_btn), "login_button");
    gtk_widget_set_opacity(GTK_WIDGET(t_authorization.login_btn), 0.5);
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.login_btn), 200, 50);
    gtk_widget_set_halign(GTK_WIDGET(t_authorization.login_btn), GTK_ALIGN_CENTER);
    g_signal_connect(G_OBJECT(t_authorization.login_btn), "enter-notify-event",
        G_CALLBACK(login_btn_enter_notify), NULL);
    g_signal_connect(G_OBJECT(t_authorization.login_btn), "leave-notify-event",
        G_CALLBACK(login_btn_leave_notify), NULL);
    g_signal_connect(G_OBJECT(t_authorization.login_btn), "button_press_event",
        G_CALLBACK(authorization), t_authorization.fail_auto_inscription);

    // Link to registration
    GtkWidget *registration_link = gtk_event_box_new();
    GtkWidget *registration_label = gtk_label_new(text_for_labels[20]);
    gtk_widget_set_name(GTK_WIDGET(registration_label), "Registration_label");
    gtk_label_set_max_width_chars(GTK_LABEL(registration_label), 32);
    gtk_widget_set_margin_start(GTK_WIDGET(registration_link), 100);
    gtk_widget_set_margin_end(GTK_WIDGET(registration_link), 100);
    gtk_container_add(GTK_CONTAINER(registration_link), registration_label);
    gtk_box_pack_start(GTK_BOX(t_authorization.log_in_menu), registration_link, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(registration_link), "Registration");
    g_signal_connect(G_OBJECT(registration_link), "enter-notify-event",
        G_CALLBACK(activate_prelight), registration_label);
    g_signal_connect(G_OBJECT(registration_link), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), registration_label);
    g_signal_connect(G_OBJECT(registration_link), "button_press_event",
        G_CALLBACK(hide_authorization_click), t_authorization.fail_inscription); ///////////

 
    //=================================================================================

    // Registration menu page 1
    //=================================================================================
    t_authorization.registration_menu_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_authorization_menu), t_authorization.registration_menu_1, FALSE, FALSE, 0);
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.registration_menu_1), 400, 400);

    // Inscription Registration
    GtkWidget *registration_inscription = gtk_label_new(text_for_labels[21]);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_1), registration_inscription, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(registration_inscription), "authorization");
    
    // Entry for a login during registration
    t_authorization.login_reg = gtk_entry_new();
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.login_reg), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.login_reg), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.login_reg), "entry_data");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.login_reg), 400, 50);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.login_reg), text_for_labels[22]);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_1), t_authorization.login_reg, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(t_authorization.login_reg), "changed",
        G_CALLBACK(data_change_registration_event), NULL);

    // Entry for a password during registration
    GtkWidget *password_reg_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(password_reg_box), "password_eye_back");
    gtk_widget_set_margin_start(GTK_WIDGET(password_reg_box), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(password_reg_box), 20);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_1), password_reg_box, FALSE, FALSE, 30);
    
    t_authorization.password_reg = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(t_authorization.password_reg), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.password_reg), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.password_reg), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.password_reg), "password_reg");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.password_reg), 335, 0);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.password_reg), text_for_labels[23]);
    gtk_box_pack_start(GTK_BOX(password_reg_box), t_authorization.password_reg, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(t_authorization.password_reg), "changed",
        G_CALLBACK(data_change_registration_event), NULL);

    GtkWidget *eye_reg_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(eye_reg_eventbox), "eye_password");
    gtk_box_pack_start(GTK_BOX(password_reg_box), eye_reg_eventbox, FALSE, FALSE, 5);
    gtk_widget_set_size_request(GTK_WIDGET(eye_reg_eventbox), 35, 30);
    gtk_widget_set_valign(GTK_WIDGET(eye_reg_eventbox), GTK_ALIGN_END);
    g_signal_connect(G_OBJECT(eye_reg_eventbox), "button_press_event",
        G_CALLBACK(eye_pressed), t_authorization.password_reg);

    // Entry to confirm a password during registration
    GtkWidget *password_reg_confirm_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(password_reg_confirm_box), "password_eye_back");
    gtk_widget_set_margin_start(GTK_WIDGET(password_reg_confirm_box), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(password_reg_confirm_box), 20);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_1), password_reg_confirm_box, FALSE, FALSE, 0);

    t_authorization.password_reg_confirm = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(t_authorization.password_reg_confirm), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.password_reg_confirm), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.password_reg_confirm), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.password_reg_confirm), "password_reg_confirm");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.password_reg_confirm), 335, 0);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.password_reg_confirm), text_for_labels[24]);
    gtk_box_pack_start(GTK_BOX(password_reg_confirm_box), t_authorization.password_reg_confirm, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(t_authorization.password_reg_confirm), "changed",
        G_CALLBACK(data_change_registration_event), NULL);
    
    GtkWidget *eye_reg_confirm_eventbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(eye_reg_confirm_eventbox), "eye_password");
    gtk_box_pack_start(GTK_BOX(password_reg_confirm_box), eye_reg_confirm_eventbox, FALSE, FALSE, 5);
    gtk_widget_set_size_request(GTK_WIDGET(eye_reg_confirm_eventbox), 35, 30);
    gtk_widget_set_valign(GTK_WIDGET(eye_reg_confirm_eventbox), GTK_ALIGN_END);
    g_signal_connect(G_OBJECT(eye_reg_confirm_eventbox), "button_press_event",
        G_CALLBACK(eye_pressed), t_authorization.password_reg_confirm);

    // Inscription fail
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_1), t_authorization.fail_inscription, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.fail_inscription), "error");

    // Back button
    GtkWidget *registration_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_1), registration_buttons_box, TRUE, FALSE, 0);
    gtk_widget_set_valign(GTK_WIDGET(registration_buttons_box), GTK_ALIGN_END);
    GtkWidget *back_btn = gtk_event_box_new();
    GtkWidget *back_btn_label = gtk_label_new(text_for_labels[15]);
    gtk_container_add(GTK_CONTAINER(back_btn), back_btn_label);
    gtk_box_pack_start(GTK_BOX(registration_buttons_box), back_btn, TRUE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(back_btn), "back_button");
    gtk_widget_set_size_request(GTK_WIDGET(back_btn), 100, 40);

    g_signal_connect(G_OBJECT(back_btn), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(back_btn), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(back_btn), "button_press_event",
        G_CALLBACK(hide_registration_click), t_authorization.fail_auto_inscription);

    // Next button
    t_authorization.next_btn = gtk_event_box_new();
    GtkWidget *next_btn_label = gtk_label_new(text_for_labels[25]);
    gtk_container_add(GTK_CONTAINER(t_authorization.next_btn), next_btn_label);
    gtk_box_pack_start(GTK_BOX(registration_buttons_box), t_authorization.next_btn, TRUE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.next_btn), "back_button");
    gtk_widget_set_opacity(GTK_WIDGET(t_authorization.next_btn), 0.5);
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.next_btn), 100, 40);

    g_signal_connect(G_OBJECT(t_authorization.next_btn), "enter-notify-event",
        G_CALLBACK(next_btn_enter_notify), t_authorization.password_reg_confirm);
    g_signal_connect(G_OBJECT(t_authorization.next_btn), "leave-notify-event",
        G_CALLBACK(next_btn_leave_notify), t_authorization.password_reg_confirm);
    g_signal_connect(G_OBJECT(t_authorization.next_btn), "button_press_event",
        G_CALLBACK(transition_registration_click), t_authorization.fail_inscription);

    g_signal_connect(G_OBJECT(t_authorization.login_reg), "activate",
        G_CALLBACK(drag_a_focus), t_authorization.password_reg);
    g_signal_connect(G_OBJECT(t_authorization.password_reg), "activate",
        G_CALLBACK(drag_a_focus), t_authorization.password_reg_confirm);
    g_signal_connect(G_OBJECT(t_authorization.password_reg_confirm), "activate",
        G_CALLBACK(transition_registration_click), t_authorization.fail_inscription);
    //=================================================================================
    

    // Registration menu page 2
    //=================================================================================
    t_authorization.registration_menu_2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_authorization_menu), t_authorization.registration_menu_2, FALSE, FALSE, 0);
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.registration_menu_2), 420, 400);

    // Inscription Registration
    GtkWidget *registration_inscription_2 = gtk_label_new(text_for_labels[21]);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_2), registration_inscription_2, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(registration_inscription_2), "authorization");

    // Entry for a first name
    t_authorization.firstname_reg = gtk_entry_new();
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.firstname_reg), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.firstname_reg), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.firstname_reg), "entry_data");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.firstname_reg), 400, 50);
    gtk_widget_set_margin_top(GTK_WIDGET(t_authorization.firstname_reg), 35);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.firstname_reg), text_for_labels[26]);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_2), t_authorization.firstname_reg, FALSE, FALSE, 0);

    // Entry for a second name
    t_authorization.secondname_reg = gtk_entry_new();
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.secondname_reg), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(t_authorization.secondname_reg), TRUE);
    gtk_widget_set_name(GTK_WIDGET(t_authorization.secondname_reg), "entry_data");
    gtk_widget_set_size_request(GTK_WIDGET(t_authorization.secondname_reg), 400, 50);
    gtk_entry_set_placeholder_text(GTK_ENTRY(t_authorization.secondname_reg), text_for_labels[27]);
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_2), t_authorization.secondname_reg, FALSE, FALSE, 0);

    // Back button
    GtkWidget *registration_buttons_box_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(GTK_WIDGET(registration_buttons_box_2), "registration_buttons_box_2");
    gtk_box_pack_start(GTK_BOX(t_authorization.registration_menu_2), registration_buttons_box_2, FALSE, FALSE, 90);
    GtkWidget *back_btn_2 = gtk_event_box_new();
    GtkWidget *back_btn_label_2 = gtk_label_new(text_for_labels[15]);
    gtk_container_add(GTK_CONTAINER(back_btn_2), back_btn_label_2);
    gtk_box_pack_start(GTK_BOX(registration_buttons_box_2), back_btn_2, TRUE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(back_btn_2), "back_button");
    gtk_widget_set_size_request(GTK_WIDGET(back_btn_2), 100, 40);

    g_signal_connect(G_OBJECT(back_btn_2), "enter-notify-event",
        G_CALLBACK(activate_prelight), NULL);
    g_signal_connect(G_OBJECT(back_btn_2), "leave-notify-event",
        G_CALLBACK(deactivate_prelight), NULL);
    g_signal_connect(G_OBJECT(back_btn_2), "button_press_event",
        G_CALLBACK(hide_registration_click_2), t_authorization.fail_inscription);

    // Finish button
    GtkWidget *finish_btn = gtk_event_box_new();
    GtkWidget *finish_btn_label = gtk_label_new(text_for_labels[28]);
    gtk_container_add(GTK_CONTAINER(finish_btn), finish_btn_label);
    gtk_box_pack_start(GTK_BOX(registration_buttons_box_2), finish_btn, TRUE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(finish_btn), "back_button");
    gtk_widget_set_opacity(GTK_WIDGET(finish_btn), 0.5);
    gtk_widget_set_size_request(GTK_WIDGET(finish_btn), 100, 40);

    g_signal_connect(G_OBJECT(finish_btn), "enter-notify-event",
        G_CALLBACK(activate_prelight_with_condition_entry), t_authorization.firstname_reg);
    g_signal_connect(G_OBJECT(finish_btn), "leave-notify-event",
        G_CALLBACK(deactivate_prelight_with_condition_entry), t_authorization.firstname_reg);
    g_signal_connect(G_OBJECT(finish_btn), "button_press_event",
        G_CALLBACK(authorization_after_registration), t_authorization.fail_auto_inscription);

    g_signal_connect(G_OBJECT(t_authorization.firstname_reg), "changed",
        G_CALLBACK(data_change_registration_event_2), finish_btn);

    g_signal_connect(G_OBJECT(t_authorization.firstname_reg), "activate",
        G_CALLBACK(drag_a_focus), t_authorization.secondname_reg);
    //=================================================================================

    gtk_widget_show_all(GTK_WIDGET(authorization_area));
    if (chat_area != NULL)
        gtk_widget_hide(GTK_WIDGET(chat_area));
    gtk_widget_hide(GTK_WIDGET(t_authorization.registration_menu_1));
    gtk_widget_hide(GTK_WIDGET(t_authorization.fail_inscription));
    gtk_widget_hide(GTK_WIDGET(t_authorization.fail_auto_inscription));
    gtk_widget_hide(GTK_WIDGET(t_authorization.registration_menu_2));
}

static void drag_a_focus(GtkWidget *widget, GtkWidget *aim) {
    gtk_widget_set_can_focus(GTK_WIDGET(aim), TRUE);
    gtk_widget_grab_focus(GTK_WIDGET(aim));
}
