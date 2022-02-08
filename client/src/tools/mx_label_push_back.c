#include "../../inc/uchat_client.h"

void mx_label_push_back(t_labels **list, void *data, int index) {
    t_labels *back_list = mx_label_create_node(data, index);
    t_labels *temp = NULL;
    if (list == NULL || *list == NULL)
        *list = back_list;
    else {
        temp = *list;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = back_list;
    }
}
