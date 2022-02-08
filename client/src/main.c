#include "../inc/uchat_client.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    mx_init_global_vars();
    argv_ptr = argv;
    mx_connect_to_server(&sockfd);
    mx_get_language_arr();
    
    mx_init_user();
    gtk_init(&argc, &argv);

    cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "client/css/standard/uchat.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Create a new window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    mx_init_window();

    // Create a main area where all widgets will be shown
    mx_configure_main_area();

    gtk_widget_show_all (window);
    mx_create_registration_menu();

    gtk_main();
    mx_free_data();

    close(sockfd);
    if (sock_for_rooms != 0)
        close(sock_for_rooms);
    return 0;
}
