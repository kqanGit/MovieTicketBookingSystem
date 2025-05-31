/**
 * @file ILogoutService.h
 * @brief Logout service interface definition
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef ILOGOUT_SERVICE_H
#define ILOGOUT_SERVICE_H

#include <memory>
#include "../context/IUserContext.h"

/**
 * @interface ILogoutService
 * @brief Interface for user logout operations
 * 
 * The ILogoutService interface defines the contract for handling user logout
 * operations in the movie ticket booking system. This interface follows the
 * Service pattern, providing a clean abstraction for logout-related business logic.
 * 
 * @details Key Responsibilities:
 * - Handle user session termination
 * - Create appropriate guest context after logout
 * - Manage user state transitions
 * - Provide clean separation between authentication logic and UI
 * 
 * Design Pattern: Service Pattern
 * - Encapsulates logout business logic
 * - Provides abstraction for different logout implementations
 * - Enables dependency injection and testing
 * - Separates concerns between UI and business logic
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<ILogoutService> logoutService = 
 *     std::make_unique<LogoutService>();
 * 
 * // Perform logout operation
 * auto guestContext = logoutService->logout();
 * 
 * // Continue with guest context
 * sessionManager.setContext(std::move(guestContext));
 * @endcode
 * 
 * @see LogoutService
 * @see IUserContext
 * @see SessionManager
 * @since 1.0
 */
class ILogoutService {
public:
    /**
     * @brief Performs user logout and returns guest context
     * 
     * Terminates the current user session and creates a new guest context
     * for continued system usage. This method handles the transition from
     * authenticated user state to guest state.
     * 
     * @return Unique pointer to a guest user context
     * 
     * @post Current user session is terminated
     * @post Returns a valid guest context
     * @post System is ready for guest operations or new login
     * 
     * @throw std::runtime_error if context creation fails
     * 
     * @note This method is safe to call multiple times
     * @note The returned context allows guest-level operations
     * @note Any user-specific data should be cleared during logout
     * 
     * Usage:
     * @code
     * try {
     *     auto guestContext = logoutService->logout();
     *     // Successfully logged out, now in guest mode
     *     sessionManager.setContext(std::move(guestContext));
     * } catch (const std::exception& e) {
     *     // Handle logout failure
     * }
     * @endcode
     */
    virtual std::unique_ptr<IUserContext> logout() = 0;
    
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     * 
     * Ensures that concrete logout service implementations are properly
     * destroyed when accessed through the interface pointer.
     */
    virtual ~ILogoutService() = default;
};

#endif
