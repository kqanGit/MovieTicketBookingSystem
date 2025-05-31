/**
 * @file UserContextCreator.h
 * @brief Concrete factory for creating authenticated user contexts
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef USER_CONTEXT_CREATOR_H
#define USER_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "../model/User.h"
#include "../model/AccountInformation.h"

/**
 * @class UserContextCreator
 * @brief Concrete factory for creating authenticated user contexts
 * 
 * This class implements the Abstract Factory pattern to create authenticated user contexts.
 * It provides factory methods for creating User objects that implement the IUserContext
 * interface, enabling authenticated users to access full system functionality.
 * 
 * @details
 * Key Features:
 * - Creates authenticated user contexts with full privileges
 * - Integrates with account information and authentication system
 * - Supports visitor pattern for role-based operations
 * - Manages user session and context lifecycle
 * 
 * Design Patterns:
 * - **Abstract Factory**: Concrete implementation for user context creation
 * - **Factory Method**: Provides multiple creation methods for different scenarios
 * - **Strategy**: User contexts encapsulate user-specific behavior
 * - **Dependency Injection**: Injects account information into user contexts
 * 
 * User Capabilities:
 * - Full booking system access
 * - Personal booking history management
 * - Profile and preference management
 * - Secure transaction processing
 * 
 * @par Usage Example
 * @code
 * AccountInformation userInfo{userId, "john_doe", "John Doe", "john@email.com"};
 * std::unique_ptr<UserContextFactory> factory = std::make_unique<UserContextCreator>();
 * auto userContext = factory->CreateUser(userInfo);
 * 
 * // User can access full functionality
 * userContext->bookTickets(movieId, seats);
 * auto bookings = userContext->getBookingHistory();
 * @endcode
 * 
 * @note User contexts require valid authentication before creation
 * @warning Account information must be validated before context creation
 * 
 * @see UserContextFactory
 * @see User
 * @see IUserContext
 * @see AccountInformation
 * @see GuestContextCreator
 */
class UserContextCreator : public UserContextFactory {
public:
    /**
     * @brief Default constructor
     * 
     * Initializes the user context creator with default settings.
     * Prepares the factory for creating authenticated user contexts.
     * 
     * @post Factory is ready to create user contexts with account information
     */
    UserContextCreator() = default;
    
    /**
     * @brief Create authenticated user context with account information
     * 
     * Creates a new User object that implements IUserContext interface using
     * provided account information. This is the primary method for creating
     * authenticated user contexts after successful login.
     * 
     * @param info Account information containing user credentials and profile data
     * @return std::unique_ptr<IUserContext> Pointer to a new authenticated User context
     * 
     * @pre Account information must be valid and authenticated
     * @pre User must exist in the database
     * @post Created context has full user permissions
     * @post Context is initialized with user's account data
     * 
     * @par User Context Features
     * - Access to booking system functionality
     * - Personal booking history and management
     * - Profile and preference settings
     * - Secure transaction capabilities
     * - Session management support
     * 
     * @throws std::invalid_argument If account information is invalid
     * @throws std::runtime_error If user context creation fails
     * 
     * @see AccountInformation
     * @see User
     * @see IUserContext
     */
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
    
    /**
     * @brief Create user context without account information (NOT RECOMMENDED)
     * 
     * This method exists to satisfy the UserContextFactory interface but is not
     * recommended for regular user context creation. It may create a context with
     * limited functionality or default user settings.
     * 
     * @return std::unique_ptr<IUserContext> Pointer to a new User context with default settings
     * 
     * @post Created context may have limited functionality
     * @post Context requires subsequent initialization with account data
     * 
     * @note This method is provided for interface compliance
     * @warning Prefer using CreateUser(const AccountInformation&) for full functionality
     * @deprecated Consider using CreateUser(const AccountInformation&) instead
     * 
     * @see CreateUser(const AccountInformation&)
     */
    std::unique_ptr<IUserContext> CreateUser() override;

};

#endif
