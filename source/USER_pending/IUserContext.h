// IUserContext.h
#ifndef IUSERCONTEXT_H
#define IUSERCONTEXT_H

#include "AccountInformation.h"
#include "IUserInformationService.h"
#include <memory> // Thêm để sử dụng unique_ptr

// Forward declarations
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

    // Các service, trả về nullptr nếu không hỗ trợ ở vai trò hiện tại
    virtual IUserInformationService* getUserInformationService() = 0; // User/Admin
    virtual IMovieViewerService* getMovieViewerService() = 0;         // Guest/User/Admin
    virtual IMovieManagerService* getMovieManagerService() = 0;       // Admin
    virtual IBookingService* getBookingService() = 0;                 // User/Admin
    virtual IViewBookingHistoryService* getViewBookingHistoryService() = 0; // User/Admin
    virtual ILoginService* getLoginService() = 0;                     // Guest
    virtual ILogoutService* getLogoutService() = 0;                   // User/Admin
    virtual IRegisterService* getRegisterService() = 0;               // Guest

    virtual ~IUserContext() = default;
};
#endif