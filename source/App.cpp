#include "App.h"
#include <iostream>
#include <string>
#include "USER_pending/MovieManagerService.h"
#include "USER_pending/BookingService.h"
#include "USER_pending/ViewBookingHistoryService.h"
#include "USER_pending/MovieViewerService.h"
#include "SessionManager.h" // Moved from USER_pending to the root folder
#include "USER_pending/LoginService.h"
#include "USER_pending/RegisterService.h"
#include <filesystem>

App::App(bool useMock) : dbConn(nullptr), authRepo(nullptr), useMockRepo(useMock) {
}

App::~App() {
    if (authRepo) {
        delete authRepo;
        authRepo = nullptr;
    }
    // dbConn is a singleton so no need to delete
}

bool App::initialize() {
    std::cout << "Initializing application..." << std::endl;
    dbConn = DatabaseConnection::getInstance();
    bool dbExists = std::filesystem::exists("database.db");
    if (!dbConn->connect("database.db")) {
        std::cerr << "Cannot connect to database." << std::endl;
        return false;
    }
    if (useMockRepo) {
        // Use mock repository for testing
        std::cout << "Using mock repository for testing" << std::endl;
        //authRepo = new MockAuthRepository();
        // For now, use the real repository
        authRepo = new AuthenticationRepositorySQL(dbConn);
    } else {
        // Chỉ chạy file SQL nếu database.db chưa tồn tại
        if (!dbExists) {
            dbConn->executeSQLFile("database/database.sql");
        }
        // Initialize SQL repository
        authRepo = new AuthenticationRepositorySQL(dbConn);
    }
    // Initialize SessionManager (no longer takes repository directly)
    sessionManager = std::make_unique<SessionManager>();    
    std::cout << "Initialization complete." << std::endl;
    return true;
}

void App::run() {
    std::cout << "Welcome to the Movie Ticket Booking System!" << std::endl;
    
    bool running = true;    
    while(running) {        // Get current user context
        auto currentUser = sessionManager->getCurrentContext();
          std::cout << "\n----- MAIN MENU -----" << std::endl;
        std::cout << "Current role: " << sessionManager->getCurrentRole() << std::endl;
          // Menu based on role
        if (currentUser->getRole() == "guest") {
            std::cout << "1. Login" << std::endl;
            std::cout << "2. Register" << std::endl;
            std::cout << "3. View movie list" << std::endl;
        } else if (currentUser->getRole() == "user") {
            std::cout << "1. View personal information" << std::endl;
            std::cout << "2. View movie list" << std::endl;
            std::cout << "3. Book tickets" << std::endl;
            std::cout << "4. View booking history" << std::endl;
            std::cout << "5. Logout" << std::endl;
        } else if (currentUser->getRole() == "admin") {
            std::cout << "1. Manage movies" << std::endl;
            std::cout << "2. View movie list" << std::endl;
            std::cout << "3. Logout" << std::endl;
        }
        
        std::cout << "0. Exit" << std::endl;
        std::cout << "Your choice: ";
        
        int choice;
        std::cin >> choice;
          // Process GUEST menu
        if (currentUser->getRole() == "guest") {
            switch(choice) {                
                case 1: { // Login
                    std::string username, password;
                    std::cout << "Username (admin/admin123, user/user123): ";
                    std::cin >> username;
                    std::cout << "Password: ";
                    std::cin >> password;
                      // Use LoginService to authenticate
                    LoginService loginService(authRepo);
                    auto authResult = loginService.authenticate(username, password);
                    if (authResult) {
                        // If authentication is successful, update context in SessionManager
                        sessionManager->setUserContext(*authResult);
                    } else {
                        std::cout << "Login failed: Incorrect username or password" << std::endl;
                    }
                    break;                }                case 2: { // Register
                    AccountInformation info;
                    std::cout << "Username: ";
                    std::cin >> info.userName;
                    std::cout << "Password: ";
                    std::cin >> info.password;
                    std::cout << "Email: ";
                    std::cin >> info.gmail;                    
                    std::cout << "Phone number: ";
                    std::cin >> info.phoneNumber;
                    info.role = "user"; // Default is user
                      // Use RegisterService to register
                    RegisterService registerService(authRepo);
                    if (registerService.registerUser(info)) {
                        std::cout << "Registration successful. Please login." << std::endl;
                    } else {
                        std::cout << "Registration failed." << std::endl;
                    }
                    break;                }
                case 3: { // View movie list
                    testViewMovie();
                    break;
                }
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
            }
        }        // Process USER menu
        else if (currentUser->getRole() == "user") {
            switch(choice) {
                case 1: // View personal information
                    std::cout << "Displaying personal information..." << std::endl;
                    std::cout << "This feature is not fully implemented yet." << std::endl;
                    break;
                case 2: // View movie list
                    testViewMovie();
                    break;
                case 3: // Book tickets
                    testBookingFunctionality();
                    break;
                case 4: // View booking history
                    testHistoryViewing();
                    break;                case 5: // Logout
                    sessionManager->logout();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
            }
        }        // Process ADMIN menu
        else if (currentUser->getRole() == "admin") {
            switch(choice) {
                case 1: // Manage movies
                    testMovieManagement();
                    break;
                case 2: // View movie list
                    testViewMovie();
                    break;                
                case 3: // Logout
                    sessionManager->logout();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
            }
        }
    }
}

void App::shutdown() {
    std::cout << "Closing application..." << std::endl;
    
    // Close database connection
    if (dbConn) {
        dbConn->disconnect();
    }
    
    std::cout << "Application closed. Goodbye!" << std::endl;
}

void App::testMovieManagement() {
    std::cout << "\n----- TEST MOVIE MANAGEMENT -----" << std::endl;    // Login as admin if not already logged in
    if (!sessionManager->isUserAuthenticated() || sessionManager->getCurrentRole() != "admin") {
        std::cout << "Logging in with admin account..." << std::endl;
          // Use LoginService to authenticate
        LoginService loginService(authRepo);
        auto authResult = loginService.authenticate("admin", "admin123");
        if (authResult && sessionManager->setUserContext(*authResult)) {
            std::cout << "Admin login successful." << std::endl;
        } else {
            std::cout << "Login failed. Unable to test movie management functionality." << std::endl;
            return;
        }
    }    auto currentUser = sessionManager->getCurrentContext();
    auto movieManager = currentUser->getMovieManagerService();
    if (movieManager) {
        std::cout << "Testing add new movie... (simulation)" << std::endl;
        std::cout << "Testing update movie information... (simulation)" << std::endl;
        std::cout << "Testing delete movie... (simulation)" << std::endl;
    } else {
        std::cout << "Movie manager service is not available" << std::endl;
    }
    sessionManager->logout();
}

void App::testBookingFunctionality() {
    std::cout << "\n----- TEST BOOKING FUNCTIONALITY -----" << std::endl;
    if (!sessionManager->isUserAuthenticated() || sessionManager->getCurrentRole() != "user") {
        std::cout << "Logging in with user account..." << std::endl;
          // Use LoginService to authenticate
        LoginService loginService(authRepo);
        auto authResult = loginService.authenticate("user", "user123");
        if (authResult && sessionManager->setUserContext(*authResult)) {
            std::cout << "User login successful." << std::endl;
        } else {
            std::cout << "Login failed. Unable to test booking functionality." << std::endl;
            return;
        }
    }    auto currentUser = sessionManager->getCurrentContext();
    auto bookingService = currentUser->getBookingService();
    if (bookingService) {
        std::cout << "Testing movie ticket booking... (simulation)" << std::endl;
        std::cout << "Booked tickets for The Avengers: Endgame, showtime 18:00 (simulation)." << std::endl;
        std::cout << "Booking code: " << rand() % 10000 + 1000 << std::endl;
    } else {
        std::cout << "Booking service is not available" << std::endl;
    }
    sessionManager->logout();
}

void App::testHistoryViewing() {
    std::cout << "\n----- TEST HISTORY VIEWING -----" << std::endl;
    if (!sessionManager->isUserAuthenticated() || sessionManager->getCurrentRole() != "user") {
        std::cout << "Logging in with user account..." << std::endl;
          // Use LoginService to authenticate
        LoginService loginService(authRepo);
        auto authResult = loginService.authenticate("user", "user123");
        if (authResult && sessionManager->setUserContext(*authResult)) {
            std::cout << "User login successful." << std::endl;
        } else {
            std::cout << "Login failed. Unable to test history viewing functionality." << std::endl;
            return;
        }
    }    auto currentUser = sessionManager->getCurrentContext();
    auto historyService = currentUser->getViewBookingHistoryService();
    if (historyService) {
        std::cout << "BOOKING HISTORY (simulation):" << std::endl;
        std::cout << "1. The Avengers: Endgame - Showtime 18:00 - 10/05/2025 - 2 tickets" << std::endl;
        std::cout << "2. Spider-Man: No Way Home - Showtime 13:30 - 15/05/2025 - 1 ticket" << std::endl;
    } else {
        std::cout << "History viewing service is not available" << std::endl;
    }
    sessionManager->logout();
}

void App::testViewMovie() {
    std::cout << "\n----- TEST VIEW MOVIE -----" << std::endl;
    auto currentUser = sessionManager->getCurrentContext();
    auto movieViewer = currentUser->getMovieViewerService();
    if (movieViewer) {
        std::cout << "MOVIE LIST (simulation):" << std::endl;
        std::cout << "1. The Avengers: Endgame" << std::endl;
        std::cout << "2. Spider-Man: No Way Home" << std::endl;
        std::cout << "3. Jurassic World" << std::endl;
    } else {
        std::cout << "Movie viewing service is not available" << std::endl;
    }
}