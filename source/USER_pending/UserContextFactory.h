#ifndef USER_CONTEXT_FACTORY_H
#define USER_CONTEXT_FACTORY_H

#include <memory>
#include "IUserContext.h"
#include "AccountInformation.h"

class UserContextFactory {
public:
    virtual std::unique_ptr<IUserContext> CreateUser() { return nullptr; } // Dùng cho Guest
    virtual std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) { return nullptr; } // Dùng cho User/Admin
    virtual ~UserContextFactory() = default;
};
#endif
