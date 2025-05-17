#include "Guest.h"
#include "LoginService.h"
#include "RegisterService.h"
#include "MovieViewerService.h"
#include "../repository/AuthenticationRepositorySQL.h"

Guest::Guest() {
    role = "guest"; // Khởi tạo role
    // Với repository được tạo động ở đây, ta cần dùng new
    auto repo = new AuthenticationRepositorySQL();
    loginService = std::make_unique<LoginService>(repo, new UserContextCreator(), new AdminContextCreator());
    registerService = std::make_unique<RegisterService>(repo);
    
    // Allow guests to view movies
    movieViewer = std::make_unique<MovieViewerService>();
}

Guest::Guest(IAuthenticationRepository* repo) {
    role = "guest"; // Khởi tạo role
    loginService = std::make_unique<LoginService>(repo, new UserContextCreator(), new AdminContextCreator());
    registerService = std::make_unique<RegisterService>(repo);
    
    // Allow guests to view movies
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
