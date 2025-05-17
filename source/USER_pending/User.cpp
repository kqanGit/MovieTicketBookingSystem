#include "User.h"
#include "LogoutService.h"
#include "BookingService.h"
#include "ViewBookingHistoryService.h"
#include "MovieViewerService.h"
#include <iostream>

User::User(const AccountInformation& acc, IAuthenticationRepository* repo) {
    role = "user"; // Khởi tạo role
    if (repo) {
        logoutService = std::make_unique<LogoutService>(repo);
    }
    
    // Initialize booking service with simple cout functionality
    bookingService = std::make_unique<BookingService>();
    
    // Initialize view history service with simple cout functionality
    viewHistoryService = std::make_unique<ViewBookingHistoryService>();
    
    // Initialize movie viewer service for users to browse movies
    movieViewer = std::make_unique<MovieViewerService>();
    
    std::cout << "User context created with role: " << role << std::endl;
}

IUserInformationService* User::getUserInformationService() { return nullptr; }
IMovieViewerService* User::getMovieViewerService() { return movieViewer.get(); }
IMovieManagerService* User::getMovieManagerService() { return nullptr; }
IBookingService* User::getBookingService() { return bookingService.get(); }
IViewBookingHistoryService* User::getViewBookingHistoryService() { return viewHistoryService.get(); }
ILoginService* User::getLoginService() { return nullptr; }
ILogoutService* User::getLogoutService() { return logoutService.get(); }
IRegisterService* User::getRegisterService() { return nullptr; }
