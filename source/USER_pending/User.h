#ifndef USER_H
#define USER_H

#include "IUserContext.h"
#include "Interfaces.h"
#include <memory> // Thêm để sử dụng unique_ptr

// Forward declaration
class IAuthenticationRepository;

class User : public IUserContext {
private:
    std::unique_ptr<IUserInformationService> userInfoService;
    std::unique_ptr<IMovieViewerService> movieViewer;
    std::unique_ptr<IBookingService> bookingService;
    std::unique_ptr<IViewBookingHistoryService> viewHistoryService;
    std::unique_ptr<ILogoutService> logoutService;
    std::string role;

public:
    User(const AccountInformation& acc, IAuthenticationRepository* repo = nullptr);
    ~User() override = default; // Smart pointers tự giải phóng
    
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
