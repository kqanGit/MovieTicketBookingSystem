#include <iostream>
#include <memory>
#include "USER_pending/MovieManagerService.h"
#include "USER_pending/BookingService.h"
#include "USER_pending/ViewBookingHistoryService.h"
#include "demo_app.h"

int main() {
    std::cout << "Starting Movie Ticket Booking System Test" << std::endl;
    
    // Initialize repository
    auto repo = new MockAuthRepository();
    
    // Create user manager
    UserManager userManager(repo);
    
    // Test login as admin
    std::cout << "\n--- Testing Admin Login ---" << std::endl;
    userManager.login("admin", "admin123");
    
    // Test admin functionality (movie management)
    std::cout << "\n--- Testing Admin Functionality ---" << std::endl;
    userManager.manageMovies();
    
    // Logout
    std::cout << "\n--- Testing Logout ---" << std::endl;
    userManager.logout();
    
    // Test login as regular user
    std::cout << "\n--- Testing User Login ---" << std::endl;
    userManager.login("user", "user123");
    
    // Test user functionality
    std::cout << "\n--- Testing User Booking Functionality ---" << std::endl;
    userManager.bookTicket();
    
    std::cout << "\n--- Testing User History Functionality ---" << std::endl;
    userManager.viewBookingHistory();
    
    // Logout
    std::cout << "\n--- Testing User Logout ---" << std::endl;
    userManager.logout();
    
    // Clean up
    delete repo;
    
    return 0;
}
