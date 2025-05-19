#include "DatabaseConnection.h"
#include <iostream>
#include <fstream>
#include <sstream>

DatabaseConnection* DatabaseConnection::instance = nullptr;

DatabaseConnection::DatabaseConnection() : db(nullptr) {}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

DatabaseConnection* DatabaseConnection::getInstance() {
    if (!instance) {
        instance = new DatabaseConnection();
    }
    return instance;
}

bool DatabaseConnection::connect(const std::string& dbFilePath) {
    if (sqlite3_open(dbFilePath.c_str(), &db) != SQLITE_OK) {
        std::cerr << " Error opening database: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
        return false;
    }
    return true;
}

void DatabaseConnection::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseConnection::executeNonQuery(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "[DatabaseConnection] Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!success) {
        std::cerr << "[DatabaseConnection] Failed to execute statement: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
    return success;
}

std::vector<std::map<std::string, std::string>> DatabaseConnection::executeQuery(
    const std::string& sql, const std::vector<std::string>& params
) {
    std::vector<std::map<std::string, std::string>> results;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "[DatabaseConnection] Failed to prepare query: " << sqlite3_errmsg(db) << "\n";
        return results;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    int colCount = sqlite3_column_count(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        for (int col = 0; col < colCount; ++col) {
            std::string colName = sqlite3_column_name(stmt, col);
            const unsigned char* text = sqlite3_column_text(stmt, col);
            row[colName] = text ? reinterpret_cast<const char*>(text) : "";
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

bool DatabaseConnection::executeSQLFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[DatabaseConnection] Could not open SQL file: " << filePath << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sql = buffer.str();

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "[DatabaseConnection] Failed to execute SQL file: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
