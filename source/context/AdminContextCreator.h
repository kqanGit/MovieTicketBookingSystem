/**
 * @file AdminContextCreator.h
 * @brief Factory implementation for creating administrator user contexts
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef ADMIN_CONTEXT_CREATOR_H
#define ADMIN_CONTEXT_CREATOR_H

#include "UserContextFactory.h"
#include "../model/Admin.h"
#include "../model/AccountInformation.h"

/**
 * @class AdminContextCreator
 * @brief Concrete factory for creating administrator user contexts
 * 
 * This class implements the UserContextFactory interface to create
 * administrator user contexts with elevated privileges and administrative
 * capabilities. It's part of the Factory pattern implementation for
 * user context creation.
 * 
 * @details
 * Key Features:
 * - Creates Admin user context instances
 * - Validates administrator account information
 * - Initializes administrative services and permissions
 * - Handles account information injection
 * - Supports both parameterized and default creation
 * 
 * @par Design Patterns Used
 * - Factory Pattern: Implements UserContextFactory interface
 * - Abstract Factory: Part of user context creation hierarchy
 * - Dependency Injection: Injects account information
 * - RAII: Automatic resource management with unique_ptr
 * 
 * @par Usage Example
 * @code
 * AdminContextCreator creator;
 * AccountInformation adminAccount("admin", UserType::ADMIN);
 * 
 * auto adminContext = creator.CreateUser(adminAccount);
 * // adminContext now contains a fully configured Admin instance
 * @endcode
 * 
 * @warning Only use for creating administrator contexts
 * 
 * @see UserContextFactory
 * @see Admin
 * @see AccountInformation
 * @see IUserContext
 */
class AdminContextCreator : public UserContextFactory {
public:
    /**
     * @brief Default constructor
     * 
     * Creates an AdminContextCreator instance ready to produce
     * administrator user contexts.
     * 
     * @post Creator is ready for admin context creation
     */
    AdminContextCreator() = default;

    /**
     * @brief Create default administrator context (not recommended)
     * 
     * Creates an administrator context without specific account information.
     * This method exists to satisfy the factory interface but should not
     * be used in production as it creates an incomplete admin context.
     * 
     * @return std::unique_ptr<IUserContext> Pointer to basic admin context
     * 
     * @retval Valid admin context with default/placeholder information
     * 
     * @post Created context has basic admin privileges but no account data
     * 
     * @deprecated Use CreateUser(const AccountInformation&) instead
     * @warning Created context will have limited functionality
     * 
     * @see CreateUser(const AccountInformation&) for preferred method
     */
    std::unique_ptr<IUserContext> CreateUser() override;

    /**
     * @brief Create administrator context with account information
     * 
     * Creates a fully configured administrator user context using the
     * provided account information. This is the preferred method for
     * creating admin contexts.
     * 
     * @param info Account information for the administrator
     * @return std::unique_ptr<IUserContext> Pointer to configured admin context
     * 
     * @retval Valid admin context with full account information
     * @retval nullptr if account information is invalid
     * 
     * @pre info.getUserType() == UserType::ADMIN
     * @pre info contains valid administrator credentials
     * 
     * @post Created context has full administrative privileges
     * @post Account information is properly injected
     * @post Administrative services are initialized
     * 
     * @throws std::invalid_argument if account type is not ADMIN
     * 
     * @par Account Validation
     * - Verifies account type is ADMIN
     * - Validates required account fields
     * - Initializes administrative permissions
     * - Sets up audit logging for admin actions
     * 
     * @see AccountInformation for required account data
     * @see Admin for created context capabilities
     */
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) override;
};

#endif