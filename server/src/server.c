#include "../inc/server.h"

void *recv_loop(void *data) {
    int newsocketfd = (int)(size_t)data;

    while(1)
    {
        char recvBuff[6000];
        bzero(recvBuff, 6000);

        int s = recv(newsocketfd, recvBuff, 6000, 0);
        if (s > 0) {
            char **recvData = mx_strsplit(recvBuff, '\n');

            if (!mx_strcmp(recvData[0], "SearchInit"))
                mx_search_init(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "CheckMessages"))
                mx_check_messages(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "CheckLastRoom"))
                mx_check_last_room(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "LoadMessages"))
                mx_load_messages(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "Authorization"))
                mx_authorization(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "InsertMessage"))
                mx_insert_message(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "AddImageMessage"))
                mx_add_image_message(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "GetMessageImage"))
                mx_get_image_message(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "LoadRoom"))
                mx_load_room(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "FindUser"))
                mx_find_user(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "AddUser"))
                mx_add_user(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "UpdateUserData"))
                mx_update_user_data(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "GetUsersArr"))
                mx_get_users_arr(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "SendRoomData"))
                mx_send_room_data(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "GetAvatar"))
                mx_get_avatar(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "UpdateAvatar"))
                mx_update_avatar(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "UpdateLanguage"))
                mx_update_language(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "GetLanguage"))
                mx_get_language(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "DeleteMessage"))
                mx_delete_message(recvData,  newsocketfd);
            else if (!mx_strcmp(recvData[0], "EditMessage"))
                mx_edit_message(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "GetTheme"))
                mx_get_theme(recvData, newsocketfd);
            else if (!mx_strcmp(recvData[0], "UpdateTheme"))
                mx_update_theme(recvData, newsocketfd);


            mx_del_strarr(&recvData);
        }
    }
    return NULL;
}


int main(int argc, char **argv) {
    sqlite3 *db = mx_opening_db();
    int exit = 0;
    char *message_error;
    char *sql = "CREATE TABLE IF NOT EXISTS USERS("
        "ID          INTEGER,"
        "NAME        TEXT NOT NULL, "
        "SURENAME    TEXT NOT NULL, "
        "PSEUDONIM   TEXT NOT NULL, "
        "DESCRIPTION TEXT NOT NULL, "
        "PASSWORD    TEXT NOT NULL, "
        "LANGUAGE    INTEGER, "
        "THEME       INTEGER, "
        "PHOTO       BLOB);";
    exit = sqlite3_exec(db, sql, NULL, 0, &message_error);
    if (exit != SQLITE_OK) {
        mx_write_to_log("Error to create USERS table", 2);
    }
    sql = "CREATE TABLE IF NOT EXISTS Messages(id BIGINT, \
           addresser BIGINT, destination BIGINT, Text TEXT, \
           Image BLOB, time BIGINT);";
    exit = sqlite3_exec(db, sql, NULL, 0, &message_error);
    mx_dberror(db, exit, "Error to create Messages table");
    sqlite3_close(db);
    mx_check_argv(argc, argv);
    int port = mx_atoi(argv[1]);
    if (port <= 0) {
        write(2, "Invalid port number\n", (int)mx_strlen("Invalid port number\n"));
    }
    mx_deamon();
    int listening_socket = mx_listening_socket(port);
    listen(listening_socket, INT_MAX);

    while(true) {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int newsocketfd = accept(listening_socket, (struct sockaddr *)&client, &client_len);

        if (newsocketfd < 0) {
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, recv_loop, (void *)(size_t)newsocketfd); 
    }
}
