#ifndef GUEST_CONTEXT_CREATOR_H
#define GUEST_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "Guest.h"

class GuestContextCreator : public UserContextFactory {
public:
    GuestContextCreator() = default;
    std::unique_ptr<IUserContext> CreateUser() override;
};

#endif
