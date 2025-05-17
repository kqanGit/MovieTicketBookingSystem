// IUserContext.h
#ifndef IUSERCONTEXT_H
#define IUSERCONTEXT_H

#include "AccountInformation.h"
#include <memory> // Thêm để sử dụng unique_ptr

// Forward declarations
class IUserInformationService;
class IMovieViewerService;
class IMovieManagerService;
class IBookingService;
class IViewBookingHistoryService;
class ILoginService;
class ILogoutService;
class IRegisterService;

class IUserContext {
public:
    virtual std::string getRole() const = 0;

public:
    // Chuyển sang trả về reference thay vì raw pointer
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