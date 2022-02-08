#include "../../inc/uchat_client.h"

unsigned int mx_remove_message(t_message **head, unsigned int id) {
    if (*head == NULL)
        return 0;
    if ((*head)->id == id) {
        mx_delete_message_from_db(*head);
        t_message *temp = (*head)->next;
        mx_del_message_node(head);
        *head = temp;
        mx_message_list_update_id(head);
        return id;
    }
    t_message *curr_node = *head;
    while (curr_node->next != NULL) {
        if (curr_node->next->id == id) {
            mx_delete_message_from_db(curr_node->next);
            t_message *temp = curr_node->next->next;
            mx_del_message_node(&(curr_node->next));
            curr_node->next = temp;
            mx_message_list_update_id(&curr_node->next);
            return id;
        }
        curr_node = curr_node->next;
    }
    return 0;
}
