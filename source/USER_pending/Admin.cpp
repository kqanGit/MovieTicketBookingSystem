#include "Admin.h"
#include "LogoutService.h"
#include "MovieManagerService.h"
#include "MovieViewerService.h"
#include "BookingService.h"
#include "ViewBookingHistoryService.h"
#include <iostream>

Admin::Admin(const AccountInformation& acc, IAuthenticationRepository* repo) {
    role = "admin"; // Khởi tạo role
    logoutService = std::make_unique<LogoutService>(repo);
    
    // Initialize services with simple cout functionality
    movieManager = std::make_unique<MovieManagerService>();
    movieViewer = std::make_unique<MovieViewerService>();
    bookingService = std::make_unique<BookingService>();
    viewHistoryService = std::make_unique<ViewBookingHistoryService>();
    
    std::cout << "Admin context created with role: " << role << std::endl;
}

IUserInformationService* Admin::getUserInformationService() { return nullptr; }
IMovieViewerService* Admin::getMovieViewerService() { return movieViewer.get(); }
IMovieManagerService* Admin::getMovieManagerService() { return movieManager.get(); }
IBookingService* Admin::getBookingService() { return bookingService.get(); }
IViewBookingHistoryService* Admin::getViewBookingHistoryService() { return viewHistoryService.get(); }
ILoginService* Admin::getLoginService() { return nullptr; }
ILogoutService* Admin::getLogoutService() { return logoutService.get(); }
IRegisterService* Admin::getRegisterService() { return nullptr; }
