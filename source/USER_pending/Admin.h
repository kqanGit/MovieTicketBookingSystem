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
#include "IUserInformationService.h" // Thêm dòng này để tránh lỗi incomplete type
#include <memory> // Thêm để sử dụng unique_ptr

// Forward declaration
class IAuthenticationRepository;

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
    Admin(const AccountInformation& acc, IAuthenticationRepository* repo);
    ~Admin() override = default; // Smart pointers tự giải phóng
    
    std::string getRole() const override { return role; }
    
    // Triển khai các phương thức từ interface
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
