#include "../inc/server.h"

void mx_update_theme(char **data, int sockfd) {
    sqlite3 *db = mx_opening_db();
    char sql[500];
    bzero(sql, 500);
    char *message_error;
    sprintf(sql, "UPDATE USERS SET THEME=%d WHERE ID=%d;", mx_atoi(data[1]), mx_atoi(data[2]));
    sqlite3_exec(db, sql, NULL, 0, &message_error);
    sqlite3_close(db);
}
