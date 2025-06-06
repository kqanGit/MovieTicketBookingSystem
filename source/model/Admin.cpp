#include "Admin.h"
#include <iostream>

void Admin::accept(std::shared_ptr<IVisitor> service) {
    std::shared_ptr<IServiceVisitor> serviceVisitor = std::dynamic_pointer_cast<IServiceVisitor>(service);
    if (serviceVisitor) {
        serviceVisitor->service(std::shared_ptr<Admin>(this, [](Admin*){})); // Non-owning shared_ptr
    }
}

Admin::Admin(const AccountInformation& acc) {
    infoService = std::make_shared<UserInformationService>(acc);
}

std::shared_ptr<IUserInformationService> Admin::getUserInformationService() const {
    return infoService;
}