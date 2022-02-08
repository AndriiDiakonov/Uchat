#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h> 
#include <time.h>
#include <limits.h>
#include <signal.h>

#include <sqlite3.h>

#include "../../libraries/libmx/inc/libmx.h"

void mx_check_port(int port);
int mx_listening_socket(int port);
void mx_error(char *msg, int status);
void mx_deamon(void);
void mx_write_to_log(char *msg, int stream);

bool mx_check_user(char **data);
void mx_check_room(char **data, int sockfd);
void mx_check_messages(char **data, int sockfd);
void mx_load_messages(char **data, int sockfd);
void mx_find_user(char **data, int sockfd);
void mx_add_user(char **data, int sockfd);
void mx_update_user_data(char **data, int sockfd);
void mx_get_users_arr(char **data, int sockfd);
void mx_send_room_data(char **data, int sockfd);
void mx_authorization(char **data, int sockfd);
void mx_get_avatar(char **data, int sockfd);
void mx_update_avatar(char **data, int sockfd);
void mx_insert_message(char **data, int sockfd);
void mx_load_room(char **data, int sockfd);
void mx_search_init(char **data, int sockfd);
void mx_update_language(char **data, int sockfd);
void mx_get_language(char **data, int sockfd);
void mx_delete_message(char **data, int sockfd);
void mx_edit_message(char **data, int sockfd);
void mx_add_image_message(char **data, int sockfd);
void mx_check_last_room(char **data, int sockfd);
void mx_get_image_message(char **data, int sockfd);
void mx_get_theme(char **data, int sockfd);
void mx_update_theme(char **data, int sockfd);

sqlite3 *mx_opening_db(void);
char *mx_encryption(char *str);
void mx_database_init(void);
void mx_dberror(sqlite3 *db, int status, char *msg);
void mx_write_photo_to_bd(char *path, int id);
bool mx_uint_arr_check_value(unsigned int *arr, unsigned int value, int len);
int mx_uint_array_insert(unsigned int **arr, unsigned int insert_value, int len);

void mx_recv_all(int *socket, unsigned char **buffer, size_t length);
void mx_send_all(int *socket, void *buffer, size_t length);

#endif
