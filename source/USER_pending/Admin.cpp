#include "Admin.h"
#include "UserInformationService.h"
#include "LogoutService.h"
#include "MovieManagerService.h"
#include "MovieViewerService.h"
#include "BookingService.h"
#include "ViewBookingHistoryService.h"
#include <iostream>

Admin::Admin(const AccountInformation& acc) {
    role = "admin";
    userInfoService = std::make_unique<UserInformationService>(acc); // Truyền toàn bộ AccountInformation
    logoutService = std::make_unique<LogoutService>();
    movieManager = std::make_unique<MovieManagerService>();
    movieViewer = std::make_unique<MovieViewerService>();
    bookingService = std::make_unique<BookingService>();
    viewHistoryService = std::make_unique<ViewBookingHistoryService>();
    std::cout << "Admin context created with role: " << role << std::endl;
}

std::string Admin::getRole() const { return role; }
IUserInformationService* Admin::getUserInformationService() { return userInfoService.get(); }
IMovieViewerService* Admin::getMovieViewerService() { return movieViewer.get(); }
IMovieManagerService* Admin::getMovieManagerService() { return movieManager.get(); }
IBookingService* Admin::getBookingService() { return bookingService.get(); }
IViewBookingHistoryService* Admin::getViewBookingHistoryService() { return viewHistoryService.get(); }
ILoginService* Admin::getLoginService() { return nullptr; }
ILogoutService* Admin::getLogoutService() { return logoutService.get(); }
IRegisterService* Admin::getRegisterService() { return nullptr; }
