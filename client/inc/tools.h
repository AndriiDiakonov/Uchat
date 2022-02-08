#ifndef TOOLS
#define TOOLS
#include <gtk/gtk.h>
#include <stdbool.h>
#include <limits.h>

void mx_free_data(void);
void mx_get_language_arr(void);
void mx_sort_string(char *data);
bool mx_uint_arr_check_value(unsigned int *arr, unsigned int value, int len);
int mx_uint_array_insert(unsigned int **arr, unsigned int insert_value, int len);

GdkPixbuf *mx_create_pixbuf(const gchar *filename);
GdkPixbuf *mx_size_image_down(GdkPixbuf *pixbuf, int w, int h);
GdkPixbuf *mx_get_pixbuf_with_size(char *path, int w, int h);

// Labels
//==========================================================================================
typedef struct s_labels
{
    int index;
    GtkWidget *data;
    struct s_labels *next;
} t_labels;
t_labels *labels_head;

t_labels *mx_label_create_node(void *data, int index);
void mx_label_pop_front(t_labels **head);
void mx_label_push_back(t_labels **list, void *data, int index);
//==========================================================================================

// Message
//==========================================================================================
typedef struct s_message
{
    GdkPixbuf *image;
    GdkPixbuf *orig_image;
    unsigned int id;
    char *text;
    unsigned int uid;
    struct s_message *next;
    time_t seconds;
}              t_message;

void mx_del_message_node(t_message **data);
void mx_clear_message_list(t_message **head);
void mx_message_list_update_id(t_message **head);
t_message *mx_message_search(t_message **head, unsigned int id);
unsigned int mx_message_list_size(t_message **head);
unsigned int mx_remove_message(t_message **head, unsigned int id);
t_message *mx_push_back_message(t_message **head, char *text, int uid, 
    GdkPixbuf *image, time_t seconds, int m_id);
t_message *mx_new_message_node(char *text, unsigned int uid, 
    GdkPixbuf *image, time_t seconds, int m_id);
//==========================================================================================

// Chats list
//==========================================================================================
typedef struct s_chats_list
{
    GdkPixbuf *avatar;
    GtkWidget *room;
    int uid;
    char *title;
    struct s_chats_list *next;
}              t_chats_list;

void mx_del_chat_node(t_chats_list **data);
void mx_clear_chat_list(t_chats_list **head);
t_chats_list *mx_chat_search(t_chats_list **head, int uid);
t_chats_list *mx_push_back_chat(t_chats_list **head, int uid, GdkPixbuf *image, GtkWidget *widget, char *title);
t_chats_list *mx_new_chat_node(unsigned int uid, GdkPixbuf *image, GtkWidget *widget, char *title);
//==========================================================================================

// Draw functions
//==========================================================================================
gboolean mx_draw_event_background(GtkWidget *widget, cairo_t *cr, gpointer user_data);
gboolean mx_draw_event_delimiter(GtkWidget *widget, cairo_t *cr);
gboolean mx_draw_event_image_avatar(GtkWidget *widget, cairo_t *cr, GdkPixbuf **img_data);
void draw_image(GtkWidget *widget, cairo_t *cr, GdkPixbuf *data);
void draw_image_square(GtkWidget *widget, cairo_t *cr, GdkPixbuf **data);
//==========================================================================================

#endif
