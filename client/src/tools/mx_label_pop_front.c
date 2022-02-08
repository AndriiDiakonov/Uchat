#include "../../inc/uchat_client.h"

void mx_label_pop_front(t_labels **head) {
    t_labels *temp = NULL;

    if (head == NULL || *head == NULL)
        return;
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
    } else {
        temp = (*head)->next;
        free(*head);
        *head = temp;
    }
}
