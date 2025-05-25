#include "AuthenticationRepositorySQL.h"
#include <stdexcept>

void AuthenticationRepositorySQL::addUser(const AccountInformation& info) {
    std::string sql = "INSERT INTO ACCOUNT (Password, RoleUser, Gmail, PhoneNumber, UserName) VALUES (?, ?, ?, ?, ?)";
    dbConn->executeNonQuery(sql, {info.password, info.role, info.gmail, info.phoneNumber, info.userName});
}

AccountInformation AuthenticationRepositorySQL::getUserByUserName(const std::string& username, const std::string& password) {
    std::string sql = "SELECT * FROM ACCOUNT WHERE UserName = ? AND Password = ?";
    auto results = dbConn->executeQuery(sql, {username, password});
    if (results.empty()) throw std::runtime_error("[AuthenticationRepoSQL] Invalid username or password");

    AccountInformation info;
    info.userName    = results[0]["UserName"];
    info.password    = results[0]["Password"];
    info.phoneNumber = results[0]["PhoneNumber"];
    info.gmail       = results[0]["Gmail"];
    info.role        = results[0]["RoleUser"];
    return info;
}
