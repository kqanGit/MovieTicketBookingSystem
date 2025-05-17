#include <iostream>
#include <memory>
#include "App.h"

int main() {
    std::cout << "=== Movie Ticket Booking System ===" << std::endl;
    std::cout << "This system demonstrates OOP design with SOLID principles." << std::endl;
    std::cout << "It uses the State Pattern for user context management, Repository Pattern for authentication," << std::endl;
    std::cout << "and implements placeholder services for future implementation." << std::endl;
    std::cout << std::endl;
    
    // Prompt the user to choose between mock and real database
    char choice;
    std::cout << "Use mock repository for testing? (y/n): ";
    std::cin >> choice;
    
    bool useMockRepo = (choice == 'y' || choice == 'Y');
    
    if (useMockRepo) {
        std::cout << "Using mock repository with preset accounts:" << std::endl;
        std::cout << "Admin: username='admin', password='admin123'" << std::endl;
        std::cout << "User: username='user', password='user123'" << std::endl;
    } else {
        std::cout << "Using real database repository (SQLite)" << std::endl;
    }
    
    // Create and initialize the app
    App app(useMockRepo);
    
    // Initialize the application
    if (!app.initialize()) {
        std::cerr << "Could not initialize the application. Exiting..." << std::endl;
        return 1;
    }
    
    // Run the application
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 2;
    }
    
    // Close the application
    app.shutdown();
    
    return 0;
}