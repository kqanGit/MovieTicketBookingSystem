#include "Guest.h"
#include "LoginService.h"
#include "RegisterService.h"
#include "MovieViewerService.h"
#include "../repository/AuthenticationRepositorySQL.h"
#include "UserContextCreator.h"
#include "AdminContextCreator.h"
#include "database/DatabaseConnection.h"

Guest::Guest() {
    role = "guest"; // Khởi tạo role
    auto db = DatabaseConnection::getInstance();
    auto repo = new AuthenticationRepositorySQL(db);
    loginService = std::make_unique<LoginService>(repo, new UserContextCreator(repo), new AdminContextCreator(repo));
    registerService = std::make_unique<RegisterService>(repo);
    movieViewer = std::make_unique<MovieViewerService>();
}

Guest::Guest(IAuthenticationRepository* repo) {
    role = "guest"; // Khởi tạo role
    loginService = std::make_unique<LoginService>(repo, new UserContextCreator(repo), new AdminContextCreator(repo));
    registerService = std::make_unique<RegisterService>(repo);
    movieViewer = std::make_unique<MovieViewerService>();
    std::cout << "Guest context created with role: " << role << std::endl;
}


IUserInformationService* Guest::getUserInformationService() { return nullptr; }
IMovieViewerService* Guest::getMovieViewerService() { return movieViewer.get(); }
IMovieManagerService* Guest::getMovieManagerService() { return nullptr; }
IBookingService* Guest::getBookingService() { return nullptr; }
IViewBookingHistoryService* Guest::getViewBookingHistoryService() { return nullptr; }
ILoginService* Guest::getLoginService() { return loginService.get(); }
ILogoutService* Guest::getLogoutService() { return nullptr; }
IRegisterService* Guest::getRegisterService() { return registerService.get(); }
