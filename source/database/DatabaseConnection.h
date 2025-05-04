#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

extern "C" {
    #include "sqlite3.h"
}

#include <string>

class DatabaseConnection {
private:
    sqlite3* db;
    std::string dbName;

public:
    DatabaseConnection(const std::string& databaseName);
    ~DatabaseConnection();

    sqlite3* getDB();
    bool isOpen() const;
};

#endif
