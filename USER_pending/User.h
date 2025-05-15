#ifndef USER_H
#define USER_H

#include "IUserContext.h"
#include "Interfaces.h"
#include "IAuthenticationRepository.h"

class User : public IUserContext {
private:
    IUserInformationService* userInfoService;
    IMovieViewerService* movieViewer;
    IBookingService* bookingService;
    IViewBookingHistoryService* viewHistoryService;
    ILogoutService* logoutService;

public:
    User(const AccountInformation& acc, IAuthenticationRepository* repo);
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
