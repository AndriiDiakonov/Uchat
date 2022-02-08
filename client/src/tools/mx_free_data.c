#include "../../inc/uchat_client.h"

void mx_free_data(void) {
    if (curr_room_msg_head != NULL)
        mx_clear_message_list(&curr_room_msg_head);

    if (chats_list_head != NULL)
        mx_clear_chat_list(&chats_list_head);

    if (search_list_head != NULL)
        mx_clear_chat_list(&search_list_head);

    if (t_user.avatar != NULL) {
        g_object_unref(G_OBJECT(t_user.avatar));
        t_user.avatar = NULL;
    }
    if (t_user.FirstName != NULL) {
        free(t_user.FirstName);
        t_user.FirstName = NULL;
    }
    if (t_user.SecondName != NULL) {
        free(t_user.SecondName);
        t_user.SecondName = NULL;
    }
    if (t_user.pseudonim != NULL) {
        free(t_user.pseudonim);
        t_user.pseudonim = NULL;
    }
    if (t_user.description != NULL) {
        free(t_user.description);
        t_user.description = NULL;
    }

    mx_del_strarr(&text_for_labels);
    while (labels_head != NULL)
        mx_label_pop_front(&labels_head);

    g_object_unref(cssProvider);
}
