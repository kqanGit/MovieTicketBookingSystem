// Guest.h
#ifndef GUEST_H
#define GUEST_H
#include "IUserContext.h"
#include "Interfaces.h"

class Guest : public IUserContext {
private:
    IMovieViewerService* movieViewer;
    ILoginService* loginService;
    IRegisterService* registerService;

public:
    Guest();
    Guest(IAuthenticationRepository* repo);
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