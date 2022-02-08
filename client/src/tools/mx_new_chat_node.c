#include "../../inc/uchat_client.h"

t_chats_list *mx_new_chat_node(unsigned int uid, GdkPixbuf *image, GtkWidget *widget, char *title) {
    t_chats_list *node = (t_chats_list *)malloc(sizeof(t_chats_list));
    node->room = widget;
    node->avatar = image;
    node->uid = uid;
    node->title = title;
    node->next = NULL;
    return node;
}
