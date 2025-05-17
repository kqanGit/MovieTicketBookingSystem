#include "UserContextCreator.h"
#include "User.h"
#include <memory>

std::unique_ptr<IUserContext> UserContextCreator::CreateUser(const AccountInformation& acc) {
    return std::make_unique<User>(acc);
}
