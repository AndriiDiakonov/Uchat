#include "../../inc/uchat_client.h"

void mx_del_message_node(t_message **data) {
    free((*data)->text);
    if ((*data)->image != NULL)
        g_object_unref(G_OBJECT((*data)->image));
    if ((*data)->orig_image != NULL)
        g_object_unref(G_OBJECT((*data)->orig_image));
    (*data)->next = NULL;
    free(*data);
    *data = NULL;
}
