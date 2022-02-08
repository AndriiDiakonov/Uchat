#include "../../inc/uchat_client.h"

t_message *mx_message_search(t_message **head, unsigned int id) {
    t_message *node = *head;
    while (node != NULL && node->id != id)
        node = node->next;
    return node;
}
