/**
 * @file App.h
 * @brief Main application header file for Movie Ticket Booking System
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

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
#include "UI/SFMLUIManager.h"  // Add SFML UI Manager

/**
 * @class App
 * @brief Main application class that orchestrates the entire movie ticket booking system
 * 
 * This class serves as the entry point and main controller for the Movie Ticket Booking System.
 * It manages the initialization of all services, repositories, and the main application loop.
 * 
 * @details
 * The App class implements the following responsibilities:
 * - Initialize database connections and repositories
 * - Set up the service layer and dependency injection
 * - Manage the main application lifecycle
 * - Handle top-level error management
 * - Coordinate between UI layer and business logic
 * 
 * @par Architecture Pattern
 * Entry Point Controller - provides simplified interface to complex subsystem
 * 
 * @par Design Patterns Used
 * - Facade Pattern: Simplified interface to complex subsystem
 * - Dependency Injection: Manages service dependencies
 * - Factory Pattern: Creates appropriate repository implementations
 * 
 * @see SessionManager
 * @see SFMLUIManager
 * @see DatabaseConnection
 */
class App {
private:
    /**
     * @brief Session manager for handling user authentication and context
     * 
     * Manages user sessions, authentication state, and user context switching
     * between guest, regular user, and admin roles.
     */
    std::shared_ptr<SessionManager> sessionManager;
    
    /**
     * @brief SFML-based user interface manager
     * 
     * Handles all graphical user interface operations including window management,
     * event handling, and rendering of UI components.
     */
    std::unique_ptr<SFMLUIManager> uiManager;
    
    /**
     * @brief Database connection instance
     * 
     * Singleton instance for managing SQLite database connections and operations.
     * @warning This is a raw pointer to a singleton - do not delete manually
     */
    DatabaseConnection* dbConn;
    
    /**
     * @brief Authentication repository for user login/registration operations
     * 
     * Handles user authentication, registration, and credential validation
     * through the repository pattern abstraction.
     */
    std::shared_ptr<IAuthenticationRepository> _authRepository;
    
    /**
     * @brief Movie repository for movie data management
     * 
     * Manages movie information including CRUD operations, filtering,
     * and movie metadata handling.
     */
    std::shared_ptr<IMovieRepository> _movieRepository;
    
    /**
     * @brief Booking repository for ticket booking operations
     * 
     * Handles booking transactions, seat reservations, and booking
     * history management.
     */
    std::shared_ptr<IBookingRepository> _bookingRepository;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes the application with production database and repositories.
     * Sets up all necessary components for normal operation.
     */
    App();
    
    /**
     * @brief Constructor with testing support
     * 
     * @param useMock If true, uses mock repositories for testing; if false, uses production repositories
     * 
     * This constructor allows the application to be initialized with either
     * mock repositories for testing purposes or real repositories for production use.
     */
    App(bool useMock);
    
    /**
     * @brief Destructor
     * 
     * Cleans up resources, closes database connections, and performs
     * proper shutdown procedures.
     */
    ~App();

    /**
     * @brief Initialize all application components
     * 
     * Sets up database connection, repositories, services, UI components,
     * and establishes the dependency injection container.
     * 
     * @return bool True if initialization successful, false otherwise
     * 
     * @throws std::runtime_error If critical components fail to initialize
     * @throws DatabaseException If database connection cannot be established
     * 
     * @par Initialization Order
     * 1. Database connection establishment
     * 2. Repository initialization
     * 3. Service layer setup
     * 4. UI manager initialization
     * 5. Session manager setup
     */
    bool initialize();
    
    /**
     * @brief Main application event loop
     * 
     * Starts the main application loop, handles user input, processes events,
     * and manages the overall application flow until termination.
     * 
     * @note This method blocks until the application is terminated
     * 
     * @see SFMLUIManager::run()
     * @see SessionManager::processEvents()
     */
    void run();
    
    /**
     * @brief Graceful application shutdown
     * 
     * Performs cleanup operations including:
     * - Saving any pending data
     * - Closing database connections
     * - Releasing UI resources
     * - Terminating background threads
     */
    void shutdown();
};

#endif // APP_H