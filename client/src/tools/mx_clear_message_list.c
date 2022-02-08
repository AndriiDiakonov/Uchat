#include "../../inc/uchat_client.h"

void mx_clear_message_list(t_message **head) {
    t_message *curr_node = *head;
    while (curr_node != NULL) {
        t_message *temp = curr_node->next;
        mx_del_message_node(&curr_node);
        curr_node = temp;
    }
    *head = NULL;
}
