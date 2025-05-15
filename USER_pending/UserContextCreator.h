// UserContextCreator.h
#ifndef USER_CONTEXT_CREATOR_H
#define USER_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "User.h"
#include "IAuthenticationRepository.h"

class UserContextCreator : public UserContextFactory {
private:
    IAuthenticationRepository* repo;
public:
    UserContextCreator(IAuthenticationRepository* r) : repo(r) {}
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& acc) override {
        return std::make_unique<User>(acc, repo);
    }
};

#endif
