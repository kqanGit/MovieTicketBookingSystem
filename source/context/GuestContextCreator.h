/**
 * @file GuestContextCreator.h
 * @brief Concrete factory for creating guest user contexts
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef GUESTCONTEXTCREATOR_H
#define GUESTCONTEXTCREATOR_H

#include "UserContextFactory.h"
#include "../model/Guest.h"
#include "../model/AccountInformation.h"
#include <stdexcept>

/**
 * @class GuestContextCreator
 * @brief Concrete factory for creating guest user contexts
 * 
 * This class implements the Abstract Factory pattern to create guest user contexts.
 * It provides factory methods for creating Guest objects that implement the IUserContext
 * interface, enabling anonymous users to interact with the system.
 * 
 * @details
 * Key Features:
 * - Creates anonymous guest contexts without authentication
 * - Implements visitor pattern support for role-based operations
 * - Provides restricted access to system functionality
 * - Thread-safe factory operations
 * 
 * Design Patterns:
 * - **Abstract Factory**: Concrete implementation for guest context creation
 * - **Factory Method**: Provides multiple creation methods for different scenarios
 * - **Strategy**: Guest contexts encapsulate guest-specific behavior
 * 
 * @par Usage Example
 * @code
 * std::unique_ptr<UserContextFactory> factory = std::make_unique<GuestContextCreator>();
 * auto guestContext = factory->CreateUser();
 * 
 * // Guest can browse movies but cannot book tickets
 * auto movies = guestContext->getAvailableServices();
 * @endcode
 * 
 * @note Guest contexts have limited privileges and cannot perform authenticated operations
 * @warning Do not use CreateUser(AccountInformation) for guest contexts - it will throw
 * 
 * @see UserContextFactory
 * @see Guest
 * @see IUserContext
 * @see UserContextCreator
 */
class GuestContextCreator : public UserContextFactory {
public:
    /**
     * @brief Default constructor
     * 
     * Initializes the guest context creator with default settings.
     * No special configuration is required for guest context creation.
     * 
     * @post Factory is ready to create guest contexts
     */
    GuestContextCreator() = default;
    
    /**
     * @brief Create a guest user context without authentication
     * 
     * Creates a new Guest object that implements IUserContext interface.
     * This method is used for anonymous users who want to browse the system
     * without logging in.
     * 
     * @return std::unique_ptr<IUserContext> Pointer to a new Guest context
     * 
     * @post Created context has guest-level permissions
     * @post Context is ready for visitor pattern operations
     * 
     * @par Guest Capabilities
     * - Browse available movies
     * - View movie details and showtimes
     * - Access public information
     * - Cannot book tickets or access user-specific features
     * 
     * @see Guest
     * @see IUserContext
     */
    std::unique_ptr<IUserContext> CreateUser() override;
    
    /**
     * @brief Creates guest context with account information (NOT SUPPORTED)
     * 
     * This method should not be called for guest contexts as guests
     * do not have account information. It exists to satisfy the
     * UserContextFactory interface contract.
     * 
     * @param info Account information (ignored for guests)
     * @return std::unique_ptr<IUserContext> Never returns - always throws
     * 
     * @throws std::logic_error Always thrown with descriptive message
     * 
     * @warning This method always throws an exception
     * @note Use CreateUser() without parameters for guest context creation
     * 
     * @see CreateUser()
     */
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};

#endif
