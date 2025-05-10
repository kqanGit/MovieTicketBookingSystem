#include <iostream>
#include "database/DatabaseConnection.h"

int main() {
    DatabaseConnection dbConn("movies.db");
    if (!dbConn.isOpen()) {
        return 1;
    }

    sqlite3* db = dbConn.getDB();
    const char* sql = "CREATE TABLE IF NOT EXISTS Movie (id INTEGER PRIMARY KEY, title TEXT, year INTEGER);";
    char* errMsg = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created or already exists.\n";
    }

    return 0;
}