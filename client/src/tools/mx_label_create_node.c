#include "../../inc/uchat_client.h"

t_labels *mx_label_create_node(void *data, int index) {
    t_labels *new_node = NULL;

    if (!data)
        return NULL;
    else {
        new_node = (t_labels *)malloc(sizeof(t_labels));
        if (!new_node)
            return NULL;
        new_node->data = data;
        new_node->index = index;
        new_node->next = NULL;
        return new_node;
    }
}
