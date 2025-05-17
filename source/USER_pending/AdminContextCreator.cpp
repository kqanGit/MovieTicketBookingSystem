#include "AdminContextCreator.h"
#include "Admin.h"
#include <memory>

std::unique_ptr<IUserContext> AdminContextCreator::CreateUser() {
    return std::make_unique<Admin>();
}