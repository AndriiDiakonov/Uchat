#include "../inc/server.h"

void mx_update_language(char **data, int sockfd) {
    sqlite3 *db = mx_opening_db();
    char sql[256];
    bzero(sql, 256);
    char *errmsg;
    sprintf(sql, "UPDATE USERS SET LANGUAGE=%d WHERE ID=%d;", mx_atoi(data[1]), mx_atoi(data[2]));   
    int st = sqlite3_exec(db, sql, NULL, 0, &errmsg);
    mx_dberror(db, st, errmsg); 
    sqlite3_close(db);
}
