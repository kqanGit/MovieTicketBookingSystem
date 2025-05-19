// IUserContext.h
#ifndef IUSERCONTEXT_H
#define IUSERCONTEXT_H

#include "AccountInformation.h"
#include "IVisitor.h"
#include <memory> // Thêm để sử dụng unique_ptr
#include <string>

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
    virtual ~IUserContext() = default;
    virtual void accept(std::shared_ptr<IVisitor> service) = 0;
    virtual std::string getRole() const = 0; // Thêm phương thức lấy vai trò
};
#endif