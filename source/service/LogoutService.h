/**
 * @file LogoutService.h
 * @brief Concrete implementation of logout service
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef LOGOUT_SERVICE_H
#define LOGOUT_SERVICE_H

#include "ILogoutService.h"
#include "../context/GuestContextCreator.h"
#include <memory>

/**
 * @class LogoutService
 * @brief Concrete implementation of user logout operations
 * 
 * The LogoutService class provides the concrete implementation of logout
 * functionality for the movie ticket booking system. This service handles
 * the transition from any authenticated user state back to guest state,
 * ensuring proper cleanup and context creation.
 * 
 * @details Implementation Features:
 * - Creates guest context using GuestContextCreator
 * - Handles logout for all user types (User, Admin)
 * - Provides stateless operation design
 * - Thread-safe logout operations
 * 
 * Design Pattern: Service Pattern
 * - Implements ILogoutService interface
 * - Encapsulates logout business logic
 * - Uses Factory pattern for context creation
 * - Provides clean separation of concerns
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<ILogoutService> logoutService = 
 *     std::make_unique<LogoutService>();
 * 
 * // Perform logout operation
 * auto guestContext = logoutService->logout();
 * 
 * // Context is now ready for guest operations
 * sessionManager.setContext(std::move(guestContext));
 * @endcode
 * 
 * @see ILogoutService
 * @see GuestContextCreator
 * @see IUserContext
 * @since 1.0
 */
class LogoutService : public ILogoutService {
public:
    /**
     * @brief Default constructor for LogoutService
     * 
     * Creates a new LogoutService instance with default configuration.
     * The service is stateless and can be reused for multiple logout operations.
     * 
     * @post LogoutService is ready for logout operations
     * 
     * @note Service is stateless and thread-safe for concurrent use
     */
    LogoutService() = default;
    
    /**
     * @brief Performs user logout and creates guest context
     * 
     * Terminates the current user session and creates a new guest context
     * using the GuestContextCreator. This implementation handles logout
     * for all user types and ensures proper state transition.
     * 
     * @return Unique pointer to a newly created guest context
     * 
     * @post Current user session is terminated
     * @post Returns a valid guest context with appropriate permissions
     * @post System is ready for guest operations or new authentication
     * 
     * @throw std::runtime_error if guest context creation fails
     * 
     * @note Implementation uses GuestContextCreator for context creation
     * @note Operation is stateless and can be called multiple times safely
     * @note No user data cleanup is required as service is stateless
     * 
     * Implementation Details:
     * - Uses GuestContextCreator to create the guest context
     * - Does not require current user information
     * - Always returns a valid guest context
     * - Thread-safe operation
     * 
     * Usage:
     * @code
     * LogoutService logoutService;
     * 
     * try {
     *     auto guestContext = logoutService.logout();
     *     // Successfully logged out, now in guest mode
     *     
     *     // Set the new context in session manager
     *     sessionManager.setContext(std::move(guestContext));
     *     
     *     // User can now perform guest operations
     * } catch (const std::exception& e) {
     *     // Handle logout failure (rare occurrence)
     *     std::cerr << "Logout failed: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::unique_ptr<IUserContext> logout() override;
};

#endif