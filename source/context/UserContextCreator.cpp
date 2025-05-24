#include "UserContextCreator.h"
#include "../model/User.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<IUserContext> UserContextCreator::CreateUser(const AccountInformation& info) {
    return std::make_unique<User>(info);
}

std::unique_ptr<IUserContext> UserContextCreator::CreateUser() {
        throw std::logic_error("UserContextCreator: Cannot create user without account info.");
}