#include "User.h"
#include "UserInformationService.h" // Thong tin ca nhan cua User
#include "LogoutService.h"
#include "BookingService.h"
#include "ViewBookingHistoryService.h"
#include "MovieViewerService.h"
#include <iostream>

User::User(const AccountInformation& acc) {
    role = "user";
    userInfoService = std::make_unique<UserInformationService>(acc); // Truyền toàn bộ AccountInformation
    movieViewer = std::make_unique<MovieViewerService>();
    bookingService = std::make_unique<BookingService>();
    viewHistoryService = std::make_unique<ViewBookingHistoryService>();
    logoutService = std::make_unique<LogoutService>();
    std::cout << "User context created with role: " << role << std::endl;
}

// Định nghĩa phương thức getRole()
std::string User::getRole() const { return role; }

IUserInformationService* User::getUserInformationService() { return userInfoService.get(); }
IMovieViewerService* User::getMovieViewerService() { return movieViewer.get(); }
IMovieManagerService* User::getMovieManagerService() { return nullptr; }
IBookingService* User::getBookingService() { return bookingService.get(); }
IViewBookingHistoryService* User::getViewBookingHistoryService() { return viewHistoryService.get(); }
ILoginService* User::getLoginService() { return nullptr; }
ILogoutService* User::getLogoutService() { return logoutService.get(); }
IRegisterService* User::getRegisterService() { return nullptr; }