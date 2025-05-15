#include "RegisterService.h"

std::unique_ptr<IUserContext> RegisterService::registerAccount(const AccountInformation& info) {
    repo->addUser(info);
    return factory->CreateUser(info);
}
