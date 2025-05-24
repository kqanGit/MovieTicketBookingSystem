// IUserContext.h
#ifndef IUSERCONTEXT_H
#define IUSERCONTEXT_H

#include "../model/AccountInformation.h"
#include "../visitor/IVisitor.h"
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
    virtual ~IUserContext() = default;
    virtual void accept(std::shared_ptr<IVisitor> service) = 0;

    // Trả về nullptr nếu là Guest
    virtual std::shared_ptr<IUserInformationService> getUserInformationService() const = 0;
};
#endif