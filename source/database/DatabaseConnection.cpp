#include "DatabaseConnection.h"
#include <iostream>

DatabaseConnection::DatabaseConnection(const std::string& databaseName)
    : db(nullptr), dbName(databaseName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

DatabaseConnection::~DatabaseConnection() {
    if (db) {
        sqlite3_close(db);
    }
}

sqlite3* DatabaseConnection::getDB() {
    return db;
}

bool DatabaseConnection::isOpen() const {
    return db != nullptr;
}