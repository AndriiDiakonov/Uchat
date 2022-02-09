#ifndef UCHAT_CLIENT
#define UCHAT_CLIENT

#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

#include "../../libraries/libmx/inc/libmx.h"
#include "database.h"

// Window size
//==========================================================================================
#define WIN_WIDTH_MIN 915
#define WIN_HEIGHT_MIN 680
gint CUR_WIDTH;
gint CUR_HEIGHT;
gint L_FIELD_WIDTH;
//==========================================================================================

// Colors
//==========================================================================================
// Standard ones
#define MX_1F1F1F 0.12
#define MX_E1E1E1 0.20
// Colorful theme
int THEME;
//==========================================================================================
int counter;
// A message
//==========================================================================================
GtkWidget *mx_create_message(t_message *data);
void mx_add_message(GtkWidget *container, t_message *message);

GtkWidget *selected_msg_widget;
t_message *selected_msg_struct;
unsigned int curr_destination;
//==========================================================================================

// Images
//==========================================================================================
typedef struct s_img_button
{
    bool active;
    GtkWidget *box;
} t_img_button;

t_img_button settings_image;
t_img_button messages_image;
t_img_button *t_active_image;

void mx_load_images(void);
//==========================================================================================

// Log in/Registration menu
//==========================================================================================
struct s_authorization
{
    GtkWidget *log_in_menu;
    GtkWidget *authorization_container;
    GtkWidget *registration_menu_1;
    GtkWidget *registration_menu_2;
    GtkWidget *login;
    GtkWidget *password; 
    GtkWidget *login_reg;
    GtkWidget *password_reg;
    GtkWidget *password_reg_confirm;
    GtkWidget *firstname_reg;
    GtkWidget *secondname_reg;
    GtkWidget *login_btn;
    GtkWidget *next_btn;

    GtkWidget *fail_inscription;
    GtkWidget *fail_auto_inscription;
}      t_authorization;

void mx_create_registration_menu(void);

void activate_prelight_with_condition_entry(GtkWidget *widget, GdkEvent *event, gpointer *entry);
void deactivate_prelight_with_condition_entry(GtkWidget *widget, GdkEvent *event, gpointer *entry);
void authorization_close(GtkWidget *widget, GdkEventButton *event);
void password_entry_change(GtkWidget *widget);

void button_shine(GtkWidget *widget, GdkEvent *event);
void login_btn_enter_notify(void);
void login_btn_leave_notify(void);
void hide_authorization_click(GtkWidget *widget, GdkEvent *event, gpointer *data);
void hide_registration_click(GtkWidget *widget, GdkEvent *event, gpointer *data);
void next_btn_enter_notify(GtkWidget *widget, GdkEvent *event, gpointer *data);
void next_btn_leave_notify(GtkWidget *widget, GdkEvent *event, gpointer *data);
void data_change_registration_event(GtkWidget *widget, GdkEvent *event);
void transition_registration_click(GtkWidget *widget, GdkEvent *event, gpointer *data);
void hide_registration_click_2(GtkWidget *widget, GdkEvent *event, gpointer *data);

void data_change_registration_event_2(GtkWidget *widget, gpointer data);
void authorization_after_registration(GtkWidget *widget, GdkEvent *event, gpointer *data);
void authorization(GtkWidget *widget, GdkEvent *event, GtkWidget *data);
void eye_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data);
//==========================================================================================


// User info
//==========================================================================================
struct
{
    int id;
    char *FirstName;
    char *SecondName;
    char *pseudonim;
    char *description;
    GdkPixbuf *avatar;
} t_user;

void mx_change_user_name(char fName[], char sName[]);
void mx_change_user_pseudonim(char str[]);
void mx_change_user_description(char str[]);
void mx_change_user_avatar(char path[]);
void mx_init_user(void);
void mx_update_user_data_preview(void);
//==========================================================================================


// Edit user form
//==========================================================================================
struct s_edit_user
{
    GtkWidget *username;
    GtkWidget *user_pseudonim;

    // Variables that will replace old ones
    char *NewFirstName;
    char *NewSecondName;
    char *NewPseudonim;
    char *NewDescription;
    GdkPixbuf *NewAvatar;
}   t_edit_user;

void mx_create_edit_user_form(void);
void edit_user_eventbox_enter_notify(GtkWidget *widget);
void edit_user_eventbox_leave_notify(GtkWidget *widget);

void change_avatart_btn_click(GtkWidget *widget, GdkEvent *event);

void fname_entry_changed_event(GtkWidget *widget);
void commit_username_click_event(GtkWidget *widget, GdkEventButton *event, gpointer builder);

void pseudo_entry_changed_event(GtkWidget *widget);
void return_pseudonim_click_event(GtkWidget *widget, GdkEventButton *event, gpointer builder);
void commit_pseudonim_click_event(GtkWidget *widget, GdkEventButton *event, gpointer builder);

void change_description_entry_change_event(GtkWidget *widget);

void commit_edit_user_click_event(GtkWidget *widget, GdkEventButton *event);
//==========================================================================================

// Language
//==========================================================================================
GtkWidget *mx_language_create_box(char *path, char *name);
void mx_create_language_menu(void);
//==========================================================================================

// Event callback functions
//==========================================================================================
void activate_prelight(GtkWidget *widget);
void deactivate_prelight(GtkWidget *widget);
void settings_element_enter_notify(GtkWidget *widget);
void settings_element_leave_notify(GtkWidget *widget);
void close_image_click_event(GtkWidget *widget, GdkEventButton *event);

void messages_click(GtkWidget *widget, GdkEventButton *event);
void settings_click(GtkWidget *widget, GdkEventButton *event);

void blackout_destroy(GtkWidget *widget, GdkEventButton *event, GtkWidget *box);
//==========================================================================================

// Chat room
//==========================================================================================
struct s_chat_room_vars {
    GtkWidget *more_box;
    GtkWidget *right_container;
    GtkWidget *messages_box;
    GtkWidget *message_enter_area;
} t_chat_room_vars;

GtkWidget *mx_create_room(unsigned int uid, gint width, void (*func)(GtkWidget *, GdkEventButton *, gpointer));
void room_click(GtkWidget *widget, GdkEventButton *event, gpointer uid);
void room_close(GtkWidget *widget, GdkEventKey *event);
int mx_create_messages_area(void);
void mx_create_message_enter_area(void);

void mx_attach(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry);
void mx_attach_send_message_on_enter(GtkWidget *widget, void **arr);
void mx_create_attach_form(GtkWidget *entry, char *filename);

void entry_chat_fill_event(GtkWidget *widget, GdkEvent *event);
void mx_send_message(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry);
void mx_send_message_on_enter(GtkWidget *widget);

void mx_more_click(GtkWidget *widget, GdkEvent *event);
//==========================================================================================


GtkWidget *window;                      // a top-level window
GtkWidget *main_area;                   // an area that contains area with authorization form and chat area
GtkWidget *authorization_area;
GtkWidget *chat_area;
GtkCssProvider *cssProvider;
int sockfd;
int sock_for_rooms;
int sock_for_send;
char **argv_ptr;

pthread_t check_messages_id; // used to create a thread for message checking
pthread_t check_last_room_id;
int max_msg_id;

GtkWidget *entry_search;
GtkWidget *search_menu;

GtkWidget *chats_list;
t_message *curr_room_msg_head;
t_chats_list *chats_list_head;
t_chats_list *search_list_head;

GtkWidget *settings_menu;
GtkWidget *active_leftbar_container;

GtkWidget *blackout;
GtkWidget *error_revealer;
GtkWidget *tools_menu;
GtkWidget *edit_prev;

int language;
char **text_for_labels;

void mx_init_window(void);
int mx_connect_to_server(int *sock);
void mx_init_global_vars(void);
void mx_update_theme(void);
void mx_tooltip(char *str, void *data);
void *mx_run_error_pop_up(void *vargp);
int mx_destroy_popups(void);
void mx_create_theme_menu(void);

void mx_configure_main_area(void);
void mx_configure_left_header(void);
void mx_configure_chats_list(void);
void mx_configure_settings_menu_area(void);
void mx_configure_username_event_screen(GtkBuilder *builder);
void mx_configure_pseudonim_event_screen(GtkBuilder *builder);

void mx_recv_all(int *socket, unsigned char **buffer, size_t length);
void mx_send_all(int *socket, void *buffer, size_t length);

#endif
