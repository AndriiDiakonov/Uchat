#include "../../inc/uchat_client.h"

t_message *mx_new_message_node(char *text, unsigned int uid, 
    GdkPixbuf *image, time_t seconds, int m_id) {
    t_message *node = (t_message *)malloc(sizeof(t_message));
    node->text = text;
    node->image = image;
    node->uid = uid;
    node->id = m_id;
    node->next = NULL;
    node->orig_image = NULL;
    node->seconds = seconds;
    return node;
}
