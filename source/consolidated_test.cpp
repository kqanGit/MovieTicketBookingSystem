#include <iostream>
#include <memory>
#include "USER_pending/Interfaces.h"
#include "USER_pending/MovieManagerService.h"
#include "USER_pending/BookingService.h"
#include "USER_pending/ViewBookingHistoryService.h"
#include "USER_pending/User.h"
#include "USER_pending/Admin.h"
#include "USER_pending/Guest.h"
#include "USER_pending/LogoutService.h"
#include "USER_pending/LoginService.h"
#include "repository/IAuthenticationRepository.h"
#include "UserManager.h"

// Mock implementation for authentication repository
class MockAuthRepo : public IAuthenticationRepository {
public:
    void addUser(const AccountInformation& account) override {
        std::cout << "Added user: " << account.userName << std::endl;
    }
    
    std::optional<AccountInformation> getUserByUserName(const std::string& username, const std::string& password) override {
        if (username == "admin" && password == "admin123") {
            AccountInformation admin;
            admin.userName = "admin";
            admin.password = "admin123";
            admin.role = "admin";
            return admin;
        } else if (username == "user" && password == "user123") {
            AccountInformation user;
            user.userName = "user";
            user.password = "user123";
            user.role = "user";
            return user;
        }
        return std::nullopt;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "Starting consolidated test of user context and services" << std::endl;
    
    // Create repository
    auto repo = new MockAuthRepo();
    
    // Create a user manager
    UserManager userManager(repo);
    
    std::cout << "\n--- Testing services directly ---" << std::endl;
    
    std::cout << "\nInitializing MovieManagerService:" << std::endl;
    MovieManagerService movieManager;
    movieManager.addMovie();
    
    std::cout << "\nInitializing BookingService:" << std::endl;
    BookingService bookingService;
    bookingService.bookTicket();
    
    std::cout << "\nInitializing ViewBookingHistoryService:" << std::endl;
    ViewBookingHistoryService historyService;
    historyService.viewAllBookings();
    
    std::cout << "\n--- Testing login as admin ---" << std::endl;
    userManager.login("admin", "admin123");
    
    // Test movie management
    std::cout << "\n--- Testing admin movie management ---" << std::endl;
    // Call the manageMovies method directly
    auto adminContext = userManager.getCurrentUser();
    auto movieManagerService = adminContext->getMovieManagerService();
    if (movieManagerService) {
        std::cout << "Successfully accessed movie manager service" << std::endl;
        // Since we know it's a MovieManagerService, we can dynamic_cast it
        auto* manager = dynamic_cast<MovieManagerService*>(movieManagerService);
        if (manager) {
            manager->addMovie();
        }
    } else {
        std::cout << "Failed to access movie manager service" << std::endl;
    }
    
    // Logout
    std::cout << "\n--- Testing logout ---" << std::endl;
    userManager.logout();
    
    std::cout << "\n--- Testing login as user ---" << std::endl;
    userManager.login("user", "user123");
    
    // Test booking functionality
    std::cout << "\n--- Testing user booking ---" << std::endl;
    auto userContext = userManager.getCurrentUser();
    auto bookingService2 = userContext->getBookingService();
    if (bookingService2) {
        std::cout << "Successfully accessed booking service" << std::endl;
        auto* booking = dynamic_cast<BookingService*>(bookingService2);
        if (booking) {
            booking->bookTicket();
        }
    } else {
        std::cout << "Failed to access booking service" << std::endl;
    }
    
    // Test history viewing
    std::cout << "\n--- Testing user history viewing ---" << std::endl;
    auto historyService2 = userContext->getViewBookingHistoryService();
    if (historyService2) {
        std::cout << "Successfully accessed history service" << std::endl;
        auto* history = dynamic_cast<ViewBookingHistoryService*>(historyService2);
        if (history) {
            history->viewAllBookings();
        }
    } else {
        std::cout << "Failed to access history service" << std::endl;
    }
    
    // Logout
    std::cout << "\n--- Testing logout ---" << std::endl;
    userManager.logout();
    
    delete repo;
    
    return 0;
}
