#include "../inc/server.h"

void mx_edit_message(char **data, int sockfd) {
    int uid = mx_atoi(data[1]);
    int dst = mx_atoi(data[2]);
    int id = mx_atoi(data[3]);

    sqlite3 *db = mx_opening_db();
    int st = 0;
    char *err_msg = 0;
    char sql[300];
    bzero(sql, 300);
    sprintf(sql, "UPDATE Messages SET Text='%s' WHERE id=%d AND\
            ((addresser=%d OR addresser=%d) AND (destination=%d OR destination=%d));", 
            data[4], id, dst, uid, dst, uid);
    st = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    mx_dberror(db, st, err_msg);
    sqlite3_close(db);
}
