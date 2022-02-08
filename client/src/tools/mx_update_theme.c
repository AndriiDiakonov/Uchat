#include "../../inc/uchat_client.h"

void mx_update_theme(void) {
    char sendBuff[256];
    mx_memset(sendBuff, 0, 256);
    sprintf(sendBuff, "GetTheme\n%d", t_user.id);

    if(send(sockfd, sendBuff, 256, 0) == -1){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }
 

    if(recv(sockfd, &THEME, sizeof(int), 0) == 0){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    switch (THEME)
    {
    case 0:
        g_object_unref(G_OBJECT(cssProvider));
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "client/css/standard/uchat.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_widget_queue_draw(GTK_WIDGET(chat_area));
        break;   
    case 1:
        g_object_unref(G_OBJECT(cssProvider));
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "client/css/colored/uchat.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_widget_queue_draw(GTK_WIDGET(chat_area));
        break;
    default:
        break;
    }
}
