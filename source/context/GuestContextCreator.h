#ifndef GUESTCONTEXTCREATOR_H
#define GUESTCONTEXTCREATOR_H

#include "UserContextFactory.h"
#include "../model/Guest.h"
#include "../model/AccountInformation.h"
#include <stdexcept> // Thêm dòng này để dùng std::logic_error

class GuestContextCreator : public UserContextFactory {
public:
    GuestContextCreator() = default;
    std::unique_ptr<IUserContext> CreateUser() override;
    // The function below shouldnot be called
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};

#endif
