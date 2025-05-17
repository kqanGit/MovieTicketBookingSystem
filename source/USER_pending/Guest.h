// Guest.h
#ifndef GUEST_H
#define GUEST_H

#include "IAuthenticationRepository.h"
#include "ILoginService.h"
#include "ILogoutService.h"
#include "IRegisterService.h"
#include "IUserContext.h"
#include "IMovieViewerService.h"
#include <memory> // Thêm để sử dụng unique_ptr

// Forward declaration
class IAuthenticationRepository;

class Guest : public IUserContext {
private:
    std::unique_ptr<IMovieViewerService> movieViewer;
    std::unique_ptr<ILoginService> loginService;
    std::unique_ptr<IRegisterService> registerService;
    std::string role;

public:
    Guest();
    Guest(IAuthenticationRepository* repo);
    ~Guest() override = default; // Smart pointers tự giải phóng
    
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