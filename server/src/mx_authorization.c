#include "../inc/server.h"

/*
    data - recv data from client
    data[0] - Operation
    data[1] - Pseudonim/Username
    data[2] - Password
    data[3] - NULL
*/

void mx_authorization(char **data, int sockfd) {
    char *sendBuff = NULL;
    if (mx_check_user(data)) {
        sendBuff = mx_strjoin(sendBuff, "SUCCESS\n");

        char temp_buff[1024];   // Will hold all data got from data base
        bzero(temp_buff, 1024); // Initialize with '\0'

        sqlite3 *db = mx_opening_db();
        sqlite3_stmt *res;
        char sql[500];
        bzero(sql, 500);
        int st;
        sprintf(sql, "SELECT ID, NAME, SURENAME, PSEUDONIM, DESCRIPTION FROM USERS WHERE PSEUDONIM='%s';", data[1]);
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_step(res);
        sprintf(temp_buff, "%d\n%s\n%s\n%s\n%s\n",
                (int)sqlite3_column_int(res, 0),
                (char *)sqlite3_column_text(res, 1),
                (char *)sqlite3_column_text(res, 2),
                (char *)sqlite3_column_text(res, 3),
                (char *)sqlite3_column_text(res, 4));
        sqlite3_finalize(res);
        sqlite3_close(db);

        sendBuff = mx_strjoin(sendBuff, temp_buff);
        send(sockfd, sendBuff, strlen(sendBuff), 0);
    }
    else {
        sendBuff = strdup("FAIL");
        
        send(sockfd, sendBuff, strlen(sendBuff), 0);
    }
    free(sendBuff);
}
