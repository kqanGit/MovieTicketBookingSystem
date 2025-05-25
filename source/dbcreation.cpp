extern "C" {
    #include "sqlite3.h"
}

// Sole purpose to create the database file ".db" and run the schema.sql file
#include "DatabaseConnection.h"
#include <iostream>

int main() {

    DatabaseConnection* db = DatabaseConnection::getInstance();

    if (!db->connect("database.db")) {
        std::cerr << "Không thể kết nối tới cơ sở dữ liệu.\n";
        return 1;
    }

    db->executeSQLFile("./database/database.sql");

    db->disconnect();
    return 0;
}