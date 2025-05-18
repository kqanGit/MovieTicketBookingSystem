#include "Guest.h"
#include "LoginService.h"
#include "RegisterService.h"
#include "MovieViewerService.h"

Guest::Guest() {
    movieViewer = std::make_unique<MovieViewerService>();
    loginService = std::make_unique<LoginService>();
    registerService = std::make_unique<RegisterService>();
    role = "guest"; // Khởi tạo role
}

std::string Guest::getRole() const { return role; }
IUserInformationService* Guest::getUserInformationService() { return nullptr; }
IMovieViewerService* Guest::getMovieViewerService() { return movieViewer.get(); }
IMovieManagerService* Guest::getMovieManagerService() { return nullptr; }
IBookingService* Guest::getBookingService() { return nullptr; }
IViewBookingHistoryService* Guest::getViewBookingHistoryService() { return nullptr; }
ILoginService* Guest::getLoginService() { return loginService.get(); }
ILogoutService* Guest::getLogoutService() { return nullptr; }
IRegisterService* Guest::getRegisterService() { return registerService.get(); }
