#ifndef GUEST_CONTEXT_CREATOR_H
#define GUEST_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "../repository/IAuthenticationRepository.h"

class GuestContextCreator : public UserContextFactory {
private:
    IAuthenticationRepository* repo;
public:
    GuestContextCreator(IAuthenticationRepository* repo) : repo(repo) {}
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};

#endif
