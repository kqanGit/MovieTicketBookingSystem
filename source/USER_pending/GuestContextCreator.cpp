// GuestContextCreator.h
#ifndef GUEST_CONTEXT_CREATOR_H
#define GUEST_CONTEXT_CREATOR_H

#include "IAuthenticationRepository.h"
#include "UserContextFactory.h"
#include "Guest.h"

class GuestContextCreator : public UserContextFactory {
private:
    IAuthenticationRepository* repo;
public:
    GuestContextCreator(IAuthenticationRepository* r) : repo(r) {}
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation&) override {
        return std::make_unique<Guest>(repo);
    }
};

#endif