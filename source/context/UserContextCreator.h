// UserContextCreator.h
#ifndef USER_CONTEXT_CREATOR_H
#define USER_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "../model/User.h"
#include "../model/AccountInformation.h"

class UserContextCreator : public UserContextFactory {
public:
    UserContextCreator() = default;
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
    // Bổ sung override cần thiết // Không cần thiết
    std::unique_ptr<IUserContext> CreateUser() override;

};

#endif
