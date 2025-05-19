#include "Guest.h"
#include "LoginService.h"
#include "RegisterService.h"
#include "MovieViewerService.h"

void Guest::accept(std::shared_ptr<IVisitor> visitor) {
    std::shared_ptr<IServiceVisitor> serviceVisitor = std::dynamic_pointer_cast<IServiceVisitor>(visitor);
    if (serviceVisitor) {
        serviceVisitor->service(std::shared_ptr<Guest>(this, [](Guest*){})); // Non-owning shared_ptr
    }
}