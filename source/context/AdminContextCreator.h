// AdminContextCreator.h
#ifndef ADMIN_CONTEXT_CREATOR_H
#define ADMIN_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "../model/Admin.h"
#include "../model/AccountInformation.h"

class AdminContextCreator : public UserContextFactory {
public:
    AdminContextCreator() = default;

    // Bổ sung override để không còn abstract // Không dùng hàm này
    std::unique_ptr<IUserContext> CreateUser() override;

    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};


#endif