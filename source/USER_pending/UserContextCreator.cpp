#include "UserContextCreator.h"
#include "User.h"
#include <memory>

std::unique_ptr<IUserContext> UserContextCreator::CreateUser(const AccountInformation& info) {
    return std::make_unique<User>(info);
}