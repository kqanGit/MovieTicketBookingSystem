// IUserContext.h
#ifndef IUSERCONTEXT_H
#define IUSERCONTEXT_H

#include "AccountInformation.h"
#include "IUserContext.h"


class IUserContext {
public:
    virtual IUserInformationService* getUserInformationService() = 0;
    virtual IMovieViewerService* getMovieViewerService() = 0;
    virtual IMovieManagerService* getMovieManagerService() = 0;
    virtual IBookingService* getBookingService() = 0;
    virtual IViewBookingHistoryService* getViewBookingHistoryService() = 0;
    virtual ILoginService* getLoginService() = 0;
    virtual ILogoutService* getLogoutService() = 0;
    virtual IRegisterService* getRegisterService() = 0;
    virtual ~IUserContext() = default;
};
#endif