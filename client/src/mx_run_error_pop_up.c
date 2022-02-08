#include "../inc/uchat_client.h"

static void pop_up_destroy(GtkWidget *widget) {
    mx_destroy_popups();
}

void *mx_run_error_pop_up(void *vargp) {
    mx_destroy_popups();

    char *text = (char *)vargp;

    error_revealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(error_revealer), 
        GTK_REVEALER_TRANSITION_TYPE_CROSSFADE);
    gtk_revealer_set_transition_duration(GTK_REVEALER(error_revealer), 1000);
    
    GtkWidget *body = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(error_revealer), body);
    g_signal_connect(G_OBJECT(body), "button_press_event",
        G_CALLBACK(pop_up_destroy), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_name(GTK_WIDGET(box), "pop_up_error");
    gtk_container_add(GTK_CONTAINER(body), box);

    GtkWidget *label = gtk_label_new(text);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    GtkAllocation alloc;
    gtk_widget_get_allocation(GTK_WIDGET(body), &alloc);

    int width = mx_strlen(text) * 4 + 50;
    gtk_fixed_put(GTK_FIXED(main_area), error_revealer, 
        CUR_WIDTH - width - 20, 20);

    gtk_widget_show_all(GTK_WIDGET(error_revealer));
    gtk_revealer_set_reveal_child(GTK_REVEALER(error_revealer), TRUE);

    return NULL;
}
