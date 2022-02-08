#include "../inc/server.h"

/*
    data - recv data from client
    data[0] - Operation
    data[1] - NAME
    data[2] - SURNAME
    data[3] - PSEUDONIM
    data[4] - DESCRIPTION
    data[5] - ID
    data[6] - NULL
*/

void mx_update_user_data(char **data, int sockfd) {
    sqlite3 *db = mx_opening_db();
    sqlite3_stmt *res;
    char sql[500];
    bzero(sql, 500);
    int st;
    char *errmsg;
    sprintf(sql, "UPDATE USERS SET NAME='%s',"
            "SURENAME='%s',"
            "PSEUDONIM='%s',"
            "DESCRIPTION='%s' WHERE ID=%d;",
            data[1], data[2], data[3], data[4], mx_atoi(data[5]));   
    st = sqlite3_exec(db, sql, NULL, 0, &errmsg);
    mx_dberror(db, st, errmsg); 
    sqlite3_close(db);
}
