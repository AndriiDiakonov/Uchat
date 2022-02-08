#include "../inc/server.h"

sqlite3 *mx_opening_db(void) {
    sqlite3 *db;
    int status = sqlite3_open("server/data/Slave.db", &db);
    if (status != SQLITE_OK) {
        mx_write_to_log("Can`t open database.\n", 2);
    }
    return db;
}
