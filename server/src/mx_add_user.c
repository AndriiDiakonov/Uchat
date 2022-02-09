#include "../inc/server.h"

/*
    data - recv data from client
    data[0] - Operation
    data[1] - NAME
    data[2] - SURNAME
    data[3] - PSEUDONIM
    data[4] - PASSWORD
    data[5] - NULL
*/

void mx_add_user(char **data, int sockfd) {
    char *encrypted_pass = mx_encryption(mx_strdup(data[4]));

    sqlite3 *db = mx_opening_db();
    sqlite3_stmt *res;
    char sql[500];
    bzero(sql, 500);
    int st;
    char *errmsg;
    sprintf(sql, "SELECT MAX(ID) FROM USERS;");
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_step(res);
    int id = (int)sqlite3_column_int(res, 0);
    sqlite3_finalize(res);
    id++;
    char *description = " ";
    sprintf(sql, 
            "INSERT INTO USERS (ID, NAME, SURENAME, PSEUDONIM, \
            DESCRIPTION, PASSWORD, LANGUAGE, THEME) VALUES('%d',\
            '%s','%s','%s','%s','%s','%d','%d');", 
            id, data[1], data[2], data[3], description, encrypted_pass, 0, 0);   
    st = sqlite3_exec(db, sql, NULL, 0, &errmsg);
    mx_dberror(db, st, errmsg); 
    sqlite3_close(db);
    mx_write_photo_to_bd("server/data/standard_avatar.png", id);

    free(encrypted_pass);
}
