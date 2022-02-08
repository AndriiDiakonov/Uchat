#include "../../inc/uchat_client.h"

void mx_clear_chat_list(t_chats_list **head) {
    t_chats_list *curr_node = *head;
    while (curr_node != NULL) {
        t_chats_list *temp = curr_node->next;
        mx_del_chat_node(&curr_node);
        curr_node = temp;
    }
    *head = NULL;
}
