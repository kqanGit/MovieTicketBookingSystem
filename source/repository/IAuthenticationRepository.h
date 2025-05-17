#ifndef IAUTHENTICATIONREPOSITORY_H
#define IAUTHENTICATIONREPOSITORY_H

#include <string>
#include <optional>
#include "../USER_pending/AccountInformation.h"

class IAuthenticationRepository {
public:
    virtual void addUser(const AccountInformation& account) = 0;
    virtual AccountInformation getUserByUserName(const std::string& username, const std::string& password) = 0;
    virtual ~IAuthenticationRepository() = default;
};

#endif
