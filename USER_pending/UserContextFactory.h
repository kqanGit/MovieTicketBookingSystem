#ifndef USER_CONTEXT_FACTORY_H
#define USER_CONTEXT_FACTORY_H

#include <memory>
#include "IUserContext.h"
#include "AccountInformation.h"

class UserContextFactory {
public:
    virtual std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) = 0;
    virtual ~UserContextFactory() = default;
};

#endif
