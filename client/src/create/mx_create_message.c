#include "../../inc/uchat_client.h"

static void tools_click(GtkWidget *widget, GdkEventButton *event, GdkDisplay *display);
static void create_tools_menu(GdkEvent *event);
static void message_click(GtkWidget *widget, GdkEvent *event, t_message *data);
static void copy_click(GtkWidget *widget, t_message *data);
static void edit_click(GtkWidget *widget, t_message *data);
static void delete_click(GtkWidget *widget, t_message *data);
static void click_image(GtkWidget *widget, GdkEvent *event, void **arr);

GtkWidget *mx_create_message(t_message *data) {
    GtkWidget *eventbox = gtk_event_box_new();

    char *msg_time = mx_strndup(ctime(&data->seconds) + 11, 5);
    GtkWidget *time_send = gtk_label_new(msg_time);
    gtk_widget_set_name(GTK_WIDGET(time_send), "time_send_stickers");
    gtk_widget_set_margin_top(GTK_WIDGET(time_send), 5);
    gtk_widget_set_margin_bottom(GTK_WIDGET(time_send), 5);
    free(msg_time);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(eventbox), box);

    if (data->uid == (unsigned int)t_user.id) {
        gtk_widget_set_name(GTK_WIDGET(eventbox), "usr_message");
        gtk_widget_set_halign(GTK_WIDGET(time_send), GTK_ALIGN_END);
    }
    else {
        gtk_widget_set_name(GTK_WIDGET(eventbox), "message");
        gtk_widget_set_halign(GTK_WIDGET(time_send), GTK_ALIGN_START);
    }

    if (data->image != NULL) {
        GtkWidget *eventbox_image = gtk_event_box_new();
        //gtk_widget_set_name(GTK_WIDGET(eventbox_image), "eventbox_image");

        GtkWidget *image = gtk_drawing_area_new();
        data->orig_image = gdk_pixbuf_copy(data->image);
        data->orig_image = mx_size_image_down(data->orig_image, 1920, 1080);

        data->image = mx_size_image_down(data->image, 500, 350);
        gtk_widget_set_size_request(GTK_WIDGET(image), gdk_pixbuf_get_width(GDK_PIXBUF(data->image)), 
            gdk_pixbuf_get_height(GDK_PIXBUF(data->image)));
        g_signal_connect(G_OBJECT(image), "draw", G_CALLBACK(draw_image), data->image);
        gtk_container_add(GTK_CONTAINER(eventbox_image), image);
        if (data->text == NULL)
            gtk_widget_set_name(GTK_WIDGET(eventbox), NULL);
        
        gtk_box_pack_start(GTK_BOX(box), eventbox_image, FALSE, FALSE, 0);
        void **arr = g_new(void *, 2);
        arr[0] = data;
        arr[1] = eventbox;
        g_signal_connect(G_OBJECT(eventbox_image), "button_press_event",
            G_CALLBACK(click_image), arr);
        /*
        g_signal_connect(G_OBJECT(eventbox_image), "enter-notify-event",
            G_CALLBACK(activate_prelight), NULL);
        g_signal_connect(G_OBJECT(eventbox_image), "leave-notify-event",
            G_CALLBACK(deactivate_prelight), NULL);
            */
    }
    if (data->text != NULL) {
        GtkWidget *label = gtk_label_new(data->text);
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
        gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_START);
        gtk_widget_set_margin_top(GTK_WIDGET(label), 5);
        gtk_widget_set_margin_start(GTK_WIDGET(label), 10);
        gtk_widget_set_margin_end(GTK_WIDGET(label), 10);
        gtk_widget_set_margin_bottom(GTK_WIDGET(label), 5);
        gtk_widget_set_name(GTK_WIDGET(label), "text_size");

    }

    gtk_widget_set_margin_start(GTK_WIDGET(time_send), 10);
    gtk_widget_set_margin_end(GTK_WIDGET(time_send), 10);
    gtk_box_pack_start(GTK_BOX(box), time_send, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(eventbox), "button_press_event",
        G_CALLBACK(message_click), data);

    return eventbox;
}


// Click on the window to close the menu
static void tools_click(GtkWidget *widget, GdkEventButton *event, GdkDisplay *display) {
    if (tools_menu != NULL) {
        GtkAllocation alloc;
        gtk_widget_get_allocation(GTK_WIDGET(tools_menu), &alloc);
        gint x_tools, y_tools;
        gtk_window_get_position(GTK_WINDOW(tools_menu), &x_tools, &y_tools);
        selected_msg_widget = NULL;
        selected_msg_struct = NULL;
        if (event->x_root > x_tools + alloc.width || event->y_root > y_tools +alloc.height
            || event->x_root < x_tools || event->y_root < y_tools) {    
            mx_destroy_popups();
        }
    }
}

static void create_tools_menu(GdkEvent *event) {
    mx_destroy_popups();

    tools_menu = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_set_device_events(GTK_WIDGET(tools_menu), gtk_get_current_event_device(), GDK_ALL_EVENTS_MASK);
    gtk_widget_realize(GTK_WIDGET(tools_menu));
    gtk_window_set_resizable(GTK_WINDOW(tools_menu), FALSE);
    gtk_window_set_decorated(GTK_WINDOW(tools_menu), FALSE);

    gdk_window_show(gtk_widget_get_window(GTK_WIDGET(tools_menu)));
    GdkDisplay *display = gtk_widget_get_display(GTK_WIDGET(chat_area));
    GdkCursor *cursor = gdk_cursor_new_from_name (display, "default");
    GdkGrabStatus status = gdk_seat_grab(gdk_display_get_default_seat(display),
        gtk_widget_get_window(GTK_WIDGET(window)), GDK_SEAT_CAPABILITY_ALL_POINTING, TRUE,
        cursor, event, NULL, NULL);
    g_object_unref(G_OBJECT(cursor));    
    g_signal_connect(G_OBJECT(window), "button_press_event",
        G_CALLBACK(tools_click), display);
}

static void message_click(GtkWidget *widget, GdkEvent *event, t_message *data) {
    if (((GdkEventButton *)event)->type == GDK_BUTTON_PRESS 
        && ((GdkEventButton *)event)->button == 3) {
        selected_msg_widget = widget;
        selected_msg_struct = data;

        create_tools_menu(event); // Creates the window of the tools menu (saves it to the 'tools_menu')
        GtkWidget *tools_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(tools_menu), tools_container);
        gtk_widget_set_name(GTK_WIDGET(tools_container), "tools_menu");

        GtkWidget *copy_btn = gtk_button_new_with_label(text_for_labels[35]);
        gtk_box_pack_start(GTK_BOX(tools_container), copy_btn, FALSE, FALSE, 0);
        gtk_widget_set_name(GTK_WIDGET(copy_btn), "tools_button");
        gtk_button_set_relief(GTK_BUTTON(copy_btn), GTK_RELIEF_NONE);
        gtk_widget_set_size_request(GTK_WIDGET(copy_btn), 150, 30);
        g_signal_connect(G_OBJECT(copy_btn), "clicked", G_CALLBACK(copy_click), data);

        GtkWidget *edit_btn = gtk_button_new_with_label(text_for_labels[37]);
        gtk_box_pack_start(GTK_BOX(tools_container), edit_btn, FALSE, FALSE, 0);
        gtk_widget_set_name(GTK_WIDGET(edit_btn), "tools_button");
        gtk_button_set_relief(GTK_BUTTON(edit_btn), GTK_RELIEF_NONE);
        gtk_widget_set_size_request(GTK_WIDGET(edit_btn), 150, 30);
        g_signal_connect(G_OBJECT(edit_btn), "clicked", G_CALLBACK(edit_click), data);

        GtkWidget *delete_btn = gtk_button_new_with_label(text_for_labels[36]);
        gtk_widget_set_name(GTK_WIDGET(delete_btn), "tools_button");
        gtk_button_set_relief(GTK_BUTTON(delete_btn), GTK_RELIEF_NONE);
        gtk_widget_set_size_request(GTK_WIDGET(delete_btn), 150, 30);
        gtk_box_pack_start(GTK_BOX(tools_container), delete_btn, FALSE, FALSE, 0);
        g_signal_connect(G_OBJECT(delete_btn), "clicked", G_CALLBACK(delete_click), data);

        // Move window of tools to the mouse click position
        gint x = ((GdkEventButton *)event)->x_root;
        gint y = ((GdkEventButton *)event)->y_root;
        gint x_win;
        gtk_window_get_position(GTK_WINDOW(window), &x_win, NULL);
        if (x + 150 >= x_win + CUR_WIDTH)
            x -= 150;
        gtk_window_move(GTK_WINDOW(tools_menu), x, y);

        gtk_widget_show_all(GTK_WIDGET(tools_menu));
    }
    else if (((GdkEventButton *)event)->type == GDK_BUTTON_PRESS)
        mx_destroy_popups();
}

static void copy_click(GtkWidget *widget, t_message *data) {
    if (data->text != NULL) {
        GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gtk_clipboard_clear(GTK_CLIPBOARD(clipboard));
        gtk_clipboard_set_text(GTK_CLIPBOARD(clipboard), data->text, mx_strlen(data->text));
    }

    mx_destroy_popups();
}

static void edit_click(GtkWidget *widget, t_message *data) {
    if (data->text != NULL) {
        edit_prev = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(GTK_WIDGET(edit_prev), CUR_WIDTH - L_FIELD_WIDTH, 40);
        gtk_fixed_put(GTK_FIXED(chat_area), edit_prev, L_FIELD_WIDTH, CUR_HEIGHT - 90);

        GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
        gtk_box_pack_start(GTK_BOX(edit_prev), separator, FALSE, FALSE, 0);

        GtkWidget *label = gtk_label_new(data->text);
        gtk_box_pack_start(GTK_BOX(edit_prev), label, FALSE, FALSE, 0);
        gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_START);
        gtk_widget_set_margin_top(GTK_WIDGET(label), 10);
        gtk_widget_set_margin_start(GTK_WIDGET(label), 60);
        gtk_widget_set_size_request(GTK_WIDGET(edit_prev), CUR_WIDTH - L_FIELD_WIDTH, 38);

        GList *children = gtk_container_get_children(GTK_CONTAINER(t_chat_room_vars.message_enter_area));
        gtk_entry_set_text(GTK_ENTRY(g_list_nth_data(children, 1)), data->text);
        g_list_free(children);

        gtk_widget_show_all(edit_prev);
    }

    mx_destroy_popups();
}

static void delete_click(GtkWidget *widget, t_message *data) {
    gtk_widget_destroy(GTK_WIDGET(selected_msg_widget));
    selected_msg_widget = NULL;
    selected_msg_struct = NULL;
    mx_remove_message(&curr_room_msg_head, data->id);
    max_msg_id--;

    mx_destroy_popups();
}

static void click_image(GtkWidget *widget, GdkEvent *event, void **arr) {
    if (mx_destroy_popups())
        return;
    t_message *data = (t_message *)arr[0];
    GtkWidget *eventbox = (GtkWidget *)arr[1];
    if (((GdkEventButton *)event)->type == GDK_BUTTON_PRESS 
        && ((GdkEventButton *)event)->button == 1) {
            GtkWidget *msg_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
            // Configure the window
            gtk_widget_realize(GTK_WIDGET(msg_window));
            gtk_window_set_resizable(GTK_WINDOW(msg_window), FALSE);
            gtk_widget_set_size_request(msg_window, 
                gdk_pixbuf_get_width(data->orig_image),
                gdk_pixbuf_get_height(data->orig_image));
            gtk_window_set_position(GTK_WINDOW(msg_window), GTK_WIN_POS_CENTER);
            g_signal_connect(msg_window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

            GtkWidget *image = gtk_image_new_from_pixbuf(data->orig_image);
            gtk_container_add(GTK_CONTAINER(msg_window), image);

            gtk_widget_show_all(GTK_WIDGET(msg_window));
    }
    else if (((GdkEventButton *)event)->type == GDK_BUTTON_PRESS 
        && ((GdkEventButton *)event)->button == 3) {
            message_click(eventbox, event, data);
    }
}
