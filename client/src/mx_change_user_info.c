#include "../inc/uchat_client.h"

void mx_change_user_name(char fName[], char sName[]) {
    if (t_user.FirstName != NULL) { free(t_user.FirstName); t_user.FirstName = NULL; }
    t_user.FirstName = strdup(fName);
    if (strlen(sName) == 0) t_user.SecondName = NULL;
    else {
        if (t_user.SecondName != NULL) { free(t_user.SecondName); t_user.SecondName = NULL; }
        t_user.SecondName = strdup(sName);
    }
}

void mx_change_user_pseudonim(char str[]) {
    if (t_user.pseudonim != NULL) { free(t_user.pseudonim); t_user.pseudonim = NULL; }
    t_user.pseudonim = strdup(str);
}

void mx_change_user_description(char str[]) {
    if (t_user.description != NULL) { free(t_user.description); t_user.description = NULL; }
    t_user.description = strdup(str);
}

void mx_change_user_avatar(char path[]) {
    t_user.avatar = mx_get_pixbuf_with_size(path, 100, 100);
}

void mx_update_user_data_preview(void) {
    char *username_tmp = strdup(t_user.FirstName);
    username_tmp = mx_strjoin(username_tmp, " ");
    username_tmp = mx_strjoin(username_tmp, t_user.SecondName);
    gtk_label_set_text(GTK_LABEL(t_edit_user.username), username_tmp);
    free(username_tmp);
    
    gtk_widget_set_name(GTK_WIDGET(t_edit_user.username), "username_preview");
    char *tmp_pseudonim = "@";
    tmp_pseudonim = mx_strjoin(tmp_pseudonim, t_user.pseudonim);
    gtk_label_set_text(GTK_LABEL(t_edit_user.user_pseudonim), tmp_pseudonim);
    free(tmp_pseudonim);
}

void mx_init_user(void) {
    //t_user.FirstName = NULL;
    //t_user.SecondName = NULL;
    //t_user.pseudonim = NULL;
    //t_user.description = NULL;
    t_user.id = -1;
}
