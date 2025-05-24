#include "User.h"
#include <iostream>

void User::accept(std::shared_ptr<IVisitor> service) {
    std::shared_ptr<IServiceVisitor> serviceVisitor = std::dynamic_pointer_cast<IServiceVisitor>(service);
    if (serviceVisitor) {
        serviceVisitor->service(std::shared_ptr<User>(this, [](User*){})); // Non-owning shared_ptr
    }
}

std::shared_ptr<IUserInformationService> User::getUserInformationService() const {
    return infoService;
}

User::User(const AccountInformation& acc) {
    infoService = std::make_shared<UserInformationService>(acc);
}