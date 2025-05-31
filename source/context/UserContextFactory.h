/**
 * @file UserContextFactory.h
 * @brief Abstract factory for creating user context objects
 * @author Movie Ticket Booking System Team
 */

#ifndef USER_CONTEXT_FACTORY_H
#define USER_CONTEXT_FACTORY_H

#include <memory>
#include "IUserContext.h"
#include "../model/AccountInformation.h"

#include <memory>
#include "IUserContext.h"
#include "../model/AccountInformation.h"

/**
 * @class UserContextFactory
 * @brief Abstract factory for creating user context instances
 * 
 * This abstract factory defines the interface for creating different types
 * of user contexts based on user roles and authentication status. It implements
 * the Abstract Factory pattern to provide a consistent interface for user
 * context creation while allowing different implementations for each role type.
 * 
 * @details
 * **Design Patterns Implemented:**
 * - **Abstract Factory Pattern**: Creates families of related user context objects
 * - **Factory Method Pattern**: Defines interface for object creation
 * - **Strategy Pattern**: Different context creation strategies per role
 * 
 * **User Context Types:**
 * - **Guest Context**: For unauthenticated users with limited access
 * - **User Context**: For authenticated regular users with standard privileges
 * - **Admin Context**: For administrators with elevated permissions
 * 
 * **Benefits:**
 * - Consistent interface for context creation
 * - Easy to extend with new user types
 * - Encapsulates context initialization logic
 * - Supports role-based access control setup
 * 
 * @see IUserContext
 * @see UserContextCreator
 * @see AdminContextCreator
 * @see GuestContextCreator
 * @see AccountInformation
 * @since v1.0
 */
class UserContextFactory {
public:
    /**
     * @brief Creates a user context without account information
     * 
     * Factory method for creating user contexts that don't require
     * authentication, typically used for guest users with limited access.
     * 
     * @return std::unique_ptr<IUserContext> Unique pointer to the created context
     * 
     * @post Returns fully initialized user context for guest access
     * @post Context has appropriate service access restrictions
     * 
     * @note Typically used for Guest user context creation
     * @note Created context should have limited permissions
     * 
     * @see IUserContext
     * @see Guest
     * @since v1.0
     */
    virtual std::unique_ptr<IUserContext> CreateUser() = 0; // Used for Guest
    
    /**
     * @brief Creates a user context with account information
     * 
     * Factory method for creating authenticated user contexts with
     * specific account information and role-based permissions.
     * 
     * @param info Account information containing user details and role
     * 
     * @return std::unique_ptr<IUserContext> Unique pointer to the created context
     * 
     * @pre info must contain valid account information
     * @pre Account role must be properly specified
     * @post Returns fully initialized user context with appropriate permissions
     * @post Context is configured for the user's specific role
     * 
     * @throws std::invalid_argument if account information is invalid
     * @throws std::runtime_error if context creation fails
     * 
     * @note Used for authenticated User and Admin context creation
     * @note Context permissions are based on account role
     * 
     * @see IUserContext
     * @see User
     * @see Admin
     * @see AccountInformation
     * @since v1.0
     */
    virtual std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) = 0; // Used for User/Admin
    
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~UserContextFactory() = default;
};
#endif
