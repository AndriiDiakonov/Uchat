#include "../../inc/uchat_client.h"

void mx_configure_main_area(void) {
     
    main_area = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), main_area);
    gtk_widget_set_size_request(GTK_WIDGET(main_area), CUR_WIDTH, CUR_HEIGHT);    

    authorization_area = gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(main_area), authorization_area, 0, 0);
    gtk_widget_set_size_request(GTK_WIDGET(authorization_area), CUR_WIDTH, CUR_HEIGHT);
}

