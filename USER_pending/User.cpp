#include "User.h"
#include "LogoutService.h"

User::User(const AccountInformation&, IAuthenticationRepository* repo) {
    logoutService = new LogoutService(repo);
    userInfoService = nullptr;
    movieViewer = nullptr;
    bookingService = nullptr;
    viewHistoryService = nullptr;
}

IUserInformationService* User::getUserInformationService() { return userInfoService; }
IMovieViewerService* User::getMovieViewerService() { return movieViewer; }
IMovieManagerService* User::getMovieManagerService() { return nullptr; }
IBookingService* User::getBookingService() { return bookingService; }
IViewBookingHistoryService* User::getViewBookingHistoryService() { return viewHistoryService; }
ILoginService* User::getLoginService() { return nullptr; }
ILogoutService* User::getLogoutService() { return logoutService; }
IRegisterService* User::getRegisterService() { return nullptr; }
