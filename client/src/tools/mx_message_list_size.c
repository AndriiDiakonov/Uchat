#include "../../inc/uchat_client.h"

unsigned int mx_message_list_size(t_message **head) {
    if (*head == NULL)
        return 0;
    unsigned int size = 0;
    t_message *node = *head;
    while (node != NULL) {
        size++;
        node = node->next;
    }
    return size;
}
