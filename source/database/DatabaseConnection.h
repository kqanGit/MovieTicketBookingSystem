#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <vector>
#include <map>

extern "C" {
    #include "sqlite3.h"
}

class DatabaseConnection {
private:
    static DatabaseConnection* instance;
    sqlite3* db;

    DatabaseConnection(); // private constructor

public:
    //DatabaseConnection(); // private constructor
    static DatabaseConnection* getInstance();

    bool connect(const std::string& dbFilePath);
    void disconnect();

    // For INSERT, UPDATE, DELETE (no result)
    bool executeNonQuery(const std::string& sql, const std::vector<std::string>& params = {});

    // For SELECT queries (returns vector of maps: column name -> value)
    std::vector<std::map<std::string, std::string>> executeQuery(
        const std::string& sql,
        const std::vector<std::string>& params = {}
    );

    // For executing entire SQL file (e.g., schema.sql)
    bool executeSQLFile(const std::string& filePath);

    ~DatabaseConnection();
};

#endif
