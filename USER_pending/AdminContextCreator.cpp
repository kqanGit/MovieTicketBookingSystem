#include "AdminContextCreator.h"

class AdminContextCreator : public UserContextFactory {
public:
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override {
        return std::make_unique<Admin>(
            new UserInformationService(info.userName),
            new MovieViewerService(),
            new MovieManagerService(),
            new BookingService(),
            new ViewBookingHistoryService(),
            new LogoutService()
        );
    }
};

