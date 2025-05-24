#include "Guest.h"
#include "../service/LoginService.h"
#include "../service/RegisterService.h"

void Guest::accept(std::shared_ptr<IVisitor> visitor) {
    std::shared_ptr<IServiceVisitor> serviceVisitor = std::dynamic_pointer_cast<IServiceVisitor>(visitor);
    if (serviceVisitor) {
        serviceVisitor->service(std::shared_ptr<Guest>(this, [](Guest*){})); // Non-owning shared_ptr
    }
}

std::shared_ptr<IUserInformationService> Guest::getUserInformationService() const {
    return nullptr;
}