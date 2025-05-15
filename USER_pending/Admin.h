#ifndef ADMIN_H
#define ADMIN_H

#include "IUserContext.h"
#include "Interfaces.h"
#include "IAuthenticationRepository.h"

class Admin : public IUserContext {
private:
    IUserInformationService* userInfoService;
    IMovieViewerService* movieViewer;
    IMovieManagerService* movieManager;
    IBookingService* bookingService;
    IViewBookingHistoryService* viewHistoryService;
    ILogoutService* logoutService;

public:
    Admin(const AccountInformation& acc, IAuthenticationRepository* repo);
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
