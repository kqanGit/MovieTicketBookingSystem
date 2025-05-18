#ifndef USER_H
#define USER_H

#include "ILoginService.h"
#include "ILogoutService.h"
#include "IRegisterService.h"
#include "IUserContext.h"
#include "IMovieViewerService.h"
#include "IBookingService.h"
#include "IViewBookingHistoryService.h"
#include "IUserInformationService.h"

#include <memory>


class User : public IUserContext {
private:
    std::unique_ptr<IUserInformationService> userInfoService;
    std::unique_ptr<IMovieViewerService> movieViewer;
    std::unique_ptr<IBookingService> bookingService;
    std::unique_ptr<IViewBookingHistoryService> viewHistoryService;
    std::unique_ptr<ILogoutService> logoutService;
    std::string role = "user";

public:
    User(const AccountInformation& acc);

    std::string getRole() const override;
    IUserInformationService* getUserInformationService() override;
    IMovieViewerService* getMovieViewerService() override;
    IMovieManagerService* getMovieManagerService() override;
    IBookingService* getBookingService() override;
    IViewBookingHistoryService* getViewBookingHistoryService() override;
    ILoginService* getLoginService() override;
    ILogoutService* getLogoutService() override;
    IRegisterService* getRegisterService() override;
};

#endif
