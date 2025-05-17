#include "UserContextCreator.h"
#include "User.h"

std::unique_ptr<IUserContext> UserContextCreator::CreateUser() {
    return std::make_unique<User>();
}