// AdminContextCreator.h
#ifndef ADMIN_CONTEXT_CREATOR_H
#define ADMIN_CONTEXT_CREATOR_H

#include "../repository/IAuthenticationRepository.h"
#include "UserContextFactory.h"
#include "Admin.h"

class AdminContextCreator : public UserContextFactory {
private:
    IAuthenticationRepository* repo;
public:
    AdminContextCreator(IAuthenticationRepository* r) : repo(r) {}
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& acc) override {
        return std::make_unique<Admin>(acc, repo);
    }
};

#endif