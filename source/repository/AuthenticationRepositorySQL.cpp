#include "AuthenticationRepositorySQL.h"
#include <stdexcept>

void AuthenticationRepositorySQL::addUser(const AccountInformation& info) {
    std::string sql = "INSERT INTO users (Password, RoleUser, Gmail, PhoneNumber, UserName VALUES (?, ?, ?, ?, ?)";
    // dbConn->executeNonQuery(sql, {info.userName, info.password, info.phoneNumber, info.gmail, info.role});
    dbConn->executeNonQuery(sql, {info.password, info.role, info.gmail, info.phoneNumber, info.userName});
}

AccountInformation AuthenticationRepositorySQL::getUserByUserName(const std::string& username, const std::string& password) {
    std::string sql = "SELECT * FROM users WHERE username = ? AND password = ?";
    auto results = dbConn->executeQuery(sql, {username, password});
    if (results.empty()) throw std::runtime_error("Invalid username or password");

    AccountInformation info;
    info.userName = results[0]["username"];
    info.password = results[0]["password"];
    info.phoneNumber = results[0]["phoneNumber"];
    info.gmail = results[0]["gmail"];
    info.role = results[0]["role"];
    return info;
}
