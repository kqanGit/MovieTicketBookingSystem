#ifndef AUTH_REPO_SQL_H
#define AUTH_REPO_SQL_H

#include "IAuthenticationRepository.h"
#include "DatabaseConnection.h"

class AuthenticationRepositorySQL : public IAuthenticationRepository {
private:
    DatabaseConnection* dbConn;
public:
    AuthenticationRepositorySQL(DatabaseConnection* db) : dbConn(db) {}

    void addUser(const AccountInformation& info) override;
    AccountInformation getUserByUserName(const std::string& user, const std::string& pass) override;
};

#endif
