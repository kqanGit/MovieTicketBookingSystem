#include "AdminContextCreator.h"
#include "Admin.h"
#include <memory>

std::unique_ptr<IUserContext> AdminContextCreator::CreateUser(const AccountInformation& info) {
    return std::make_unique<Admin>(info);
}