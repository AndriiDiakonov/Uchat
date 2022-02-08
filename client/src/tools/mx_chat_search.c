#include "../../inc/uchat_client.h"

t_chats_list *mx_chat_search(t_chats_list **head, int uid) {
    t_chats_list *node = *head;
    while (node != NULL && node->uid != uid)
        node = node->next;
    return node;
}
