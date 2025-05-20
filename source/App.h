// App.h
#ifndef APP_H
#define APP_H

#include <memory>
#include <filesystem>
#include "SessionManager.h"
#include "repository/IAuthenticationRepository.h"
#include "repository/AuthenticationRepositorySQL.h"
#include "database/DatabaseConnection.h"
#include "IRegisterService.h"
#include "ILoginService.h"
#include "ILogoutService.h"
#include "RegisterService.h"
#include "LoginService.h"
#include "LogoutService.h"
#include "ServiceRegistry.h"

class App {
private:
    std::unique_ptr<SessionManager> sessionManager;
    DatabaseConnection* dbConn;
    IAuthenticationRepository* authRepo;

public:
    App();
    App(bool useMock); // NEW: hỗ trợ chọn mock hay thật
    ~App();

    bool initialize();
    void run();
    void shutdown();
};

#endif // APP_H