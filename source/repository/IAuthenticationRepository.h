#ifndef IAUTHENTICATIONREPOSITORY_H
#define IAUTHENTICATIONREPOSITORY_H

#include <string>
#include "../model/AccountInformation.h" // Corrected path

class IAuthenticationRepository {
public:
    virtual void addUser(const AccountInformation& account) = 0;
    virtual AccountInformation getUserByUserName(const std::string& username, const std::string& password) = 0;
    virtual ~IAuthenticationRepository() = default;
};

#endif
