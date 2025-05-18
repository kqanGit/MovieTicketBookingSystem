#ifndef ADMIN_H
#define ADMIN_H

#include "ILoginService.h"
#include "ILogoutService.h"
#include "IRegisterService.h"
#include "IUserContext.h"
#include "IMovieViewerService.h"
#include "IMovieManagerService.h"
#include "IBookingService.h"
#include "IViewBookingHistoryService.h"
#include "IUserInformationService.h"
#include <memory>

class Admin : public IUserContext {
private:
    std::unique_ptr<IUserInformationService> userInfoService;
    std::unique_ptr<IMovieViewerService> movieViewer;
    std::unique_ptr<IMovieManagerService> movieManager;
    std::unique_ptr<IBookingService> bookingService;
    std::unique_ptr<IViewBookingHistoryService> viewHistoryService;
    std::unique_ptr<ILogoutService> logoutService;
    std::string role;

public:
    Admin(const AccountInformation& acc);
    ~Admin() override = default;

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
