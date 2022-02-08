#include "../../inc/uchat_client.h"

void mx_message_list_update_id(t_message **head) {
    t_message *node = *head;
    while (node != NULL) {
        node->id -= 1;
        node = node->next;
    }
}
