#include "Admin.h"
#include "LogoutService.h"

Admin::Admin(const AccountInformation&, IAuthenticationRepository* repo) {
    logoutService = new LogoutService(repo);
    userInfoService = nullptr;
    movieViewer = nullptr;
    movieManager = nullptr;
    bookingService = nullptr;
    viewHistoryService = nullptr;
}

IUserInformationService* Admin::getUserInformationService() { return userInfoService; }
IMovieViewerService* Admin::getMovieViewerService() { return movieViewer; }
IMovieManagerService* Admin::getMovieManagerService() { return movieManager; }
IBookingService* Admin::getBookingService() { return bookingService; }
IViewBookingHistoryService* Admin::getViewBookingHistoryService() { return viewHistoryService; }
ILoginService* Admin::getLoginService() { return nullptr; }
ILogoutService* Admin::getLogoutService() { return logoutService; }
IRegisterService* Admin::getRegisterService() { return nullptr; }
