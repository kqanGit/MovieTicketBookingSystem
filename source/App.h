// App.h
#ifndef APP_H
#define APP_H

#include <memory>
#include <filesystem>
#include "SessionManager.h"
#include "repository/IAuthenticationRepository.h"
#include "repository/IMovieRepository.h" // Added for _movieRepository
#include "repository/IBookingRepository.h" // Added for _bookingRepository
#include "repository/AuthenticationRepositorySQL.h"
#include "database/DatabaseConnection.h"
#include "IRegisterService.h"
#include "ILoginService.h"
#include "ILogoutService.h"
#include "RegisterService.h"
#include "LoginService.h"
#include "LogoutService.h"
#include "ServiceRegistry.h"
#include "RegisterServiceVisitor.h"

class App {
private:
    std::unique_ptr<SessionManager> sessionManager;
    DatabaseConnection* dbConn;
    std::shared_ptr<IAuthenticationRepository> _authRepository;
    std::shared_ptr<IMovieRepository> _movieRepository;
    std::shared_ptr<IBookingRepository> _bookingRepository;

public:
    App();
    App(bool useMock); // NEW: hỗ trợ chọn mock hay thật
    ~App();

    bool initialize();
    void run();
    void shutdown();
};

#endif // APP_H