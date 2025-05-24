#include <gtest/gtest.h>
#include "../visitor/LoginServiceVisitor.h"
#include "../visitor/RegisterServiceVisitor.h"
#include "../visitor/BookingServiceVisitor.h"
#include "../visitor/LogoutServiceVisitor.h"
#include "../visitor/MovieViewerServiceVisitor.h"
#include "../visitor/MovieMangerServiceVisitor.h"
#include "../model/Guest.h"
#include "../model/User.h"
#include "../model/Admin.h"
#include "../database/DatabaseConnection.h"
#include "../repository/AuthenticationRepositorySQL.h"
#include "../repository/BookingRepositorySQL.h"
#include "../repository/MovieRepositorySQL.h"
#include "../core/ServiceRegistry.h"
#include "../service/LoginService.h"
#include "../service/RegisterService.h"
#include "../service/BookingService.h"
#include "../service/IBookingService.h"
#include "../service/LogoutService.h"
#include "../service/ILogoutService.h"
#include "../service/MovieViewerService.h"
#include "../service/IMovieViewerService.h"
#include "../service/MovieManagerService.h"
#include "../service/IMovieManagerService.h"

class VisitorServiceTest : public ::testing::Test {
protected:    void SetUp() override {
        // Initialize database connection
        db = DatabaseConnection::getInstance();
        db->connect("database.db");
        
        // Create repositories
        authRepo = new AuthenticationRepositorySQL(db);
        std::string dbPath = "database.db";
        auto bookingRepo = std::make_shared<BookingRepository>(dbPath);
        auto movieRepo = std::make_shared<MovieRepositorySQL>(dbPath);
        
        // Register all services in the registry with their repositories
        ServiceRegistry::addSingleton<ILoginService>(std::make_shared<LoginService>(authRepo));
        ServiceRegistry::addSingleton<IRegisterService>(std::make_shared<RegisterService>(authRepo));
        ServiceRegistry::addSingleton<IBookingService>(std::make_shared<BookingService>(bookingRepo));
        ServiceRegistry::addSingleton<ILogoutService>(std::make_shared<LogoutService>());
        ServiceRegistry::addSingleton<IMovieViewerService>(std::make_shared<MovieViewerService>(movieRepo));
        ServiceRegistry::addSingleton<IMovieManagerService>(std::make_shared<MovieManagerService>(movieRepo));
        
        // Create test contexts
        guestContext = std::make_shared<Guest>();
        
        // Create a user with account information
        AccountInformation userInfo = {
            "testUser", 
            "password", 
            "123456789", 
            "test@example.com", 
            "User"
        };
        userContext = std::make_shared<User>(userInfo);
        
        // Create an admin with account information
        AccountInformation adminInfo = {
            "testAdmin", 
            "password", 
            "987654321", 
            "admin@example.com", 
            "Admin"
        };
        adminContext = std::make_shared<Admin>(adminInfo);
    }
    
    void TearDown() override {
        delete authRepo;
        db->disconnect();
    }
    
    DatabaseConnection* db;
    AuthenticationRepositorySQL* authRepo;
    std::shared_ptr<Guest> guestContext;
    std::shared_ptr<User> userContext;
    std::shared_ptr<Admin> adminContext;
};

// Test: Guest can use login and register services
TEST_F(VisitorServiceTest, GuestCanLoginAndRegister) {
    // Test login service with visitor
    auto loginVisitor = std::make_shared<LoginServiceVisitor>();
    guestContext->accept(loginVisitor);
    EXPECT_NE(loginVisitor->getLoginService(), nullptr);

    // Test register service with visitor
    auto regVisitor = std::make_shared<RegisterServiceVisitor>();
    guestContext->accept(regVisitor);
    EXPECT_NE(regVisitor->getRegisterService(), nullptr);
}

// Test: Guest cannot use booking service
TEST_F(VisitorServiceTest, GuestCannotBook) {
    auto bookingVisitor = std::make_shared<BookingServiceVisitor>();
    guestContext->accept(bookingVisitor);
    
    // Guest should not have booking permission
    EXPECT_EQ(bookingVisitor->getBookingService(), nullptr);
}

// Test: Guest cannot logout (no active session)
TEST_F(VisitorServiceTest, GuestCannotLogout) {
    auto logoutVisitor = std::make_shared<LogoutServiceVisitor>();
    guestContext->accept(logoutVisitor);
    
    // Guest should not have logout permission
    EXPECT_EQ(logoutVisitor->getLogoutService(), nullptr);
}

// Test: Guest can view movies
TEST_F(VisitorServiceTest, GuestCanViewMovies) {
    auto movieViewerVisitor = std::make_shared<MovieViewerServiceVisitor>();
    guestContext->accept(movieViewerVisitor);
    
    // Guest should have movie viewing permission
    EXPECT_NE(movieViewerVisitor->getMovieViewerService(), nullptr);
}

// Test: Guest cannot manage movies
TEST_F(VisitorServiceTest, GuestCannotManageMovies) {
    auto movieManagerVisitor = std::make_shared<MovieManagerServiceVisitor>();
    guestContext->accept(movieManagerVisitor);
    
    // Guest should not have movie management permission
    EXPECT_EQ(movieManagerVisitor->getMovieManagerService(), nullptr);
}

// Test: User role permissions
TEST_F(VisitorServiceTest, UserPermissions) {
    // User cannot login (already logged in)
    auto loginVisitor = std::make_shared<LoginServiceVisitor>();
    userContext->accept(loginVisitor);
    EXPECT_EQ(loginVisitor->getLoginService(), nullptr);
    
    // User cannot register (already has an account)
    auto regVisitor = std::make_shared<RegisterServiceVisitor>();
    userContext->accept(regVisitor);
    EXPECT_EQ(regVisitor->getRegisterService(), nullptr);
    
    // User can book tickets
    auto bookingVisitor = std::make_shared<BookingServiceVisitor>();
    userContext->accept(bookingVisitor);
    EXPECT_NE(bookingVisitor->getBookingService(), nullptr);
    
    // User can logout
    auto logoutVisitor = std::make_shared<LogoutServiceVisitor>();
    userContext->accept(logoutVisitor);
    EXPECT_NE(logoutVisitor->getLogoutService(), nullptr);
    
    // User can view movies
    auto movieViewerVisitor = std::make_shared<MovieViewerServiceVisitor>();
    userContext->accept(movieViewerVisitor);
    EXPECT_NE(movieViewerVisitor->getMovieViewerService(), nullptr);
    
    // User cannot manage movies
    auto movieManagerVisitor = std::make_shared<MovieManagerServiceVisitor>();
    userContext->accept(movieManagerVisitor);
    EXPECT_EQ(movieManagerVisitor->getMovieManagerService(), nullptr);
}

// Test: Admin role permissions
TEST_F(VisitorServiceTest, AdminPermissions) {
    // Admin cannot login (already logged in)
    auto loginVisitor = std::make_shared<LoginServiceVisitor>();
    adminContext->accept(loginVisitor);
    EXPECT_EQ(loginVisitor->getLoginService(), nullptr);
    
    // Admin cannot register (already has an account)
    auto regVisitor = std::make_shared<RegisterServiceVisitor>();
    adminContext->accept(regVisitor);
    EXPECT_EQ(regVisitor->getRegisterService(), nullptr);
    
    // Admin can book tickets
    auto bookingVisitor = std::make_shared<BookingServiceVisitor>();
    adminContext->accept(bookingVisitor);
    EXPECT_NE(bookingVisitor->getBookingService(), nullptr);
    
    // Admin can logout
    auto logoutVisitor = std::make_shared<LogoutServiceVisitor>();
    adminContext->accept(logoutVisitor);
    EXPECT_NE(logoutVisitor->getLogoutService(), nullptr);
    
    // Admin can view movies
    auto movieViewerVisitor = std::make_shared<MovieViewerServiceVisitor>();
    adminContext->accept(movieViewerVisitor);
    EXPECT_NE(movieViewerVisitor->getMovieViewerService(), nullptr);
    
    // Admin can manage movies
    auto movieManagerVisitor = std::make_shared<MovieManagerServiceVisitor>();
    adminContext->accept(movieManagerVisitor);
    EXPECT_NE(movieManagerVisitor->getMovieManagerService(), nullptr);
}

// Test: User information service is available for logged in users but not guests
TEST_F(VisitorServiceTest, UserInfoServiceAvailability) {
    // Guest has no user info
    EXPECT_EQ(guestContext->getUserInformationService(), nullptr);
    
    // User has user info
    EXPECT_NE(userContext->getUserInformationService(), nullptr);
    
    // Admin has user info
    EXPECT_NE(adminContext->getUserInformationService(), nullptr);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
