// Guest.h
#ifndef GUEST_H
#define GUEST_H

#include "ILoginService.h"
#include "ILogoutService.h"
#include "IRegisterService.h"
#include "IUserContext.h"
#include "IMovieViewerService.h"
#include <memory> // Thêm để sử dụng unique_ptr

class Guest : public IUserContext {
private:
    std::unique_ptr<IMovieViewerService> movieViewer;
    std::unique_ptr<ILoginService> loginService;
    std::unique_ptr<IRegisterService> registerService;
    std::string role;

public:
    Guest(); // constructor mặc định, khởi tạo các service cần thiết

    std::string getRole() const override;
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