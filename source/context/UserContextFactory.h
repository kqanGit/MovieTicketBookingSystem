#ifndef USER_CONTEXT_FACTORY_H
#define USER_CONTEXT_FACTORY_H

#include <memory>
#include "IUserContext.h"
#include "../model/AccountInformation.h"

class UserContextFactory {
public:
    virtual std::unique_ptr<IUserContext> CreateUser() = 0; // Dùng cho Guest
    virtual std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) = 0; // Dùng cho User/Admin
    virtual ~UserContextFactory() = default;
};
#endif
