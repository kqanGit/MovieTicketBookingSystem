#ifndef GUEST_CONTEXT_CREATOR_H
#define GUEST_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "Guest.h"
#include <stdexcept> // Thêm dòng này để dùng std::logic_error

class GuestContextCreator : public UserContextFactory {
public:
    GuestContextCreator() = default;
    std::unique_ptr<IUserContext> CreateUser() override;
    // The function below shouldnot be called
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};

#endif
