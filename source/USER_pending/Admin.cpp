#include "Admin.h"
#include "UserInformationService.h"
#include "LogoutService.h"
#include "MovieManagerService.h"
#include "MovieViewerService.h"
#include "BookingService.h"
#include "ViewBookingHistoryService.h"
#include <iostream>

void Admin::accept(std::shared_ptr<IVisitor> service) {
    std::shared_ptr<IServiceVisitor> serviceVisitor = std::dynamic_pointer_cast<IServiceVisitor>(service);
    if (serviceVisitor) {
        serviceVisitor->service(std::shared_ptr<Admin>(this, [](Admin*){})); // Non-owning shared_ptr
    }
}
