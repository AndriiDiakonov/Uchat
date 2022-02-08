#include "../../inc/uchat_client.h"

void mx_del_chat_node(t_chats_list **data) {
    if ((*data)->avatar != NULL)
        g_object_unref(G_OBJECT((*data)->avatar));
    (*data)->next = NULL;
    free((*data)->title);
    free(*data);
    *data = NULL;
}
