// UserContextCreator.h
#ifndef USER_CONTEXT_CREATOR_H
#define USER_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "User.h"

class UserContextCreator : public UserContextFactory {
public:
    UserContextCreator() = default;
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};

#endif
