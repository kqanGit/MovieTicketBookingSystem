#ifndef APP_H
#define APP_H

#include <memory>
#include "USER_pending/SessionManager.h"
#include "repository/IAuthenticationRepository.h"
#include "repository/AuthenticationRepositorySQL.h"
#include "database/DatabaseConnection.h"
#include "MockAuthRepository.h"

// App class that manages the application lifecycle
class App {
private:
    std::unique_ptr<SessionManager> sessionManager;
    DatabaseConnection* dbConn;
    IAuthenticationRepository* authRepo;
    bool useMockRepo;

public:
    App(bool useMock = false);
    ~App();
    
    bool initialize();
    void run();
    void shutdown();
    
    // Helper methods for testing functionality
    void testMovieManagement();
    void testBookingFunctionality();
    void testHistoryViewing();
    void testViewMovie();
};

#endif // APP_H