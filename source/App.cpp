// App.cpp
#include "App.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits> 
#include <memory> // Required for std::shared_ptr, std::make_shared
#include <sstream> // Required for std::istringstream
#include "BookingService.h"
#include "MovieViewerService.h"
#include "MovieManagerService.h"
#include "LoginServiceVisitor.h"
#include "LogoutServiceVisitor.h"
#include "BookingServiceVisitor.h"
#include "MovieViewerServiceVisitor.h"
#include "MovieMangerServiceVisitor.h"
#include "RegisterServiceVisitor.h"
#include "repository/BookingRepositorySQL.h" // This includes BookingRepository class
#include "repository/BookingView.h"
#include "repository/SeatView.h"
#include "model/Movie.h" 
#include "repository/MovieRepositorySQL.h"
#include "repository/IMovieRepository.h" // Added to ensure IMoviezRepository is known for MovieManagerService
#include "repository/AuthenticationRepositorySQL.h" // Added for _authRepository initialization

App::App() : dbConn(nullptr) {} // Removed authRepo initialization

App::App(bool useMock = false) : dbConn(nullptr) { // Removed authRepo initialization
    if (useMock) {
        std::cout << "[App] Using mock repository (not implemented here).\n";
        // TODO: Implement mock repo if needed
    } else {
        std::cout << "[App] Using real repository.\n";
    }
}


App::~App() {
    shutdown();
}

bool App::initialize() {
    std::cout << "\n[App] Initializing application...\n";
    
    std::cout << "[Debug] Working dir: " << std::filesystem::current_path() << "\n";

    dbConn = DatabaseConnection::getInstance();
    if (!dbConn->connect("database.db")) { // Đường dẫn đến file database
        std::cerr << "[App] Failed to connect to database.\n";
        return false;
    }

    // Nếu file chưa tồn tại hoặc mới tạo -> nên luôn chạy schema
    if (!std::filesystem::exists("database.db") || dbConn->executeQuery("SELECT name FROM sqlite_master WHERE type='table';").empty()) {
        std::cout << "[App] Running database schema setup...\n";
        if (!dbConn->executeSQLFile("../database/database.sql")) {
            std::cerr << "[App] Failed to initialize database schema.\n";
            return false;
        }
    }    // Create and store shared repository instances
    _authRepository = std::make_shared<AuthenticationRepositorySQL>(dbConn);
    _movieRepository = std::make_shared<MovieRepositorySQL>("database.db"); 
    _bookingRepository = std::make_shared<BookingRepository>("database.db"); // Changed to pass string filepath

    // Register services with shared repository instances
    ServiceRegistry::addSingleton<ILoginService>(std::make_shared<LoginService>(_authRepository.get())); // Use .get()
    ServiceRegistry::addSingleton<IRegisterService>(std::make_shared<RegisterService>(_authRepository.get())); // Use .get()
    ServiceRegistry::addSingleton<ILogoutService>(std::make_shared<LogoutService>());
    ServiceRegistry::addSingleton<IBookingService>(std::make_shared<BookingService>(_bookingRepository));
    ServiceRegistry::addSingleton<IMovieViewerService>(std::make_shared<MovieViewerService>(_movieRepository));
    ServiceRegistry::addSingleton<IMovieManagerService>(std::make_shared<MovieManagerService>(_movieRepository));   
    sessionManager = std::make_shared<SessionManager>();
    
    // Initialize SFML UI Manager
    uiManager = std::make_unique<SFMLUIManager>(sessionManager);
    if (!uiManager->initialize()) {
        std::cerr << "[App] Failed to initialize SFML UI Manager.\n";
        return false;
    }
    
    std::cout << "[App] Initialization complete.\n";
    return true;
}

void App::run() {
    std::cout << "\n[App] Starting SFML UI...\n";
    
    if (!uiManager) {
        std::cerr << "[App] UI Manager not initialized!\n";
        return;
    }
    
    // Main SFML UI loop - the UI Manager handles all the console functionality
    uiManager->run();
    
    std::cout << "[App] SFML UI closed.\n";
}

void App::shutdown() {
    std::cout << "[App] Shutting down application...\n";
    // ServiceRegistry::clear(); // Optional: Clear service registry if needed
    if (dbConn) {
        dbConn->disconnect();
        // delete dbConn; // DatabaseConnection is a singleton, managed by itself or a smart pointer if applicable
        // dbConn = nullptr; // No need if DatabaseConnection::getInstance() handles lifetime
    }
    // delete authRepo; // Removed, _authRepository is a shared_ptr and manages its own lifetime
    // authRepo = nullptr; // Removed

    // _authRepository, _movieRepository, _bookingRepository are shared_ptrs, 
    // they will automatically release resources when they go out of scope 
    // or when their reference count drops to zero.
    // No explicit deletion is needed here for them.

    std::cout << "[App] Shutdown complete.\n";
}