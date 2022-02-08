#include "../inc/uchat_client.h"

// Sets the tooltip with "str" text for the "data" widget
void mx_tooltip(char *str, void *data) {
    gtk_widget_set_tooltip_text(data, str);
    gtk_widget_set_halign(data, GTK_ALIGN_START);
}

