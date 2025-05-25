#include "AdminContextCreator.h"
#include "../model/Admin.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<IUserContext> AdminContextCreator::CreateUser(const AccountInformation& info) {
    return std::make_unique<Admin>(info);
}

std::unique_ptr<IUserContext> AdminContextCreator::CreateUser()  {
        throw std::logic_error("AdminContextCreator: Cannot create user without account info.");
}