#include "Guest.h"
#include "LoginService.h"
#include "RegisterService.h"
#include "AuthenticationRepositorySQL.h"

Guest::Guest() {
    movieViewer = nullptr; // Placeholder: inject real movie viewer if needed
    loginService = new LoginService(new AuthenticationRepositorySQL());
    registerService = new RegisterService(new AuthenticationRepositorySQL());
}

Guest::Guest(IAuthenticationRepository* repo) {
    loginService = new LoginService(repo, new UserContextCreator(), new AdminContextCreator());
    registerService = new RegisterService(repo);
    movieViewer = nullptr;
}


IUserInformationService* Guest::getUserInformationService() { return nullptr; }
IMovieViewerService* Guest::getMovieViewerService() { return movieViewer; }
IMovieManagerService* Guest::getMovieManagerService() { return nullptr; }
IBookingService* Guest::getBookingService() { return nullptr; }
IViewBookingHistoryService* Guest::getViewBookingHistoryService() { return nullptr; }
ILoginService* Guest::getLoginService() { return loginService; }
ILogoutService* Guest::getLogoutService() { return nullptr; }
IRegisterService* Guest::getRegisterService() { return registerService; }
