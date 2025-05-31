/**
 * @file LoginServiceVisitor.h
 * @brief Visitor implementation for role-based login service access control
 * @author MovieTicketBookingSystem Team
 * @date 2025
 * 
 * This file implements the Visitor pattern for providing role-specific access
 * to login services within the Movie Ticket Booking System.
 */

#ifndef LOGINSERVICEVISITOR_H
#define LOGINSERVICEVISITOR_H
#include "IServiceVisitor.h"
#include "../service/ILoginService.h"
#include "../core/ServiceRegistry.h"
#include <map>
#include <memory>

class Guest;
class User;
class Admin;

/**
 * @class LoginServiceVisitor
 * @brief Concrete visitor for managing role-based access to login services
 * 
 * The LoginServiceVisitor implements the Visitor pattern to provide secure,
 * role-based access to login functionality. It ensures that login services
 * are only accessible to appropriate user roles and maintains proper
 * authentication workflows.
 * 
 * This class enforces the business rule that:
 * - Guests can access login services to authenticate
 * - Users can access login services for re-authentication
 * - Admins can access login services for administrative authentication
 * 
 * @par Design Pattern
 * - **Visitor Pattern**: Separates login service access logic from role objects
 * - **Service Locator**: Uses ServiceRegistry for dependency resolution
 * - **Singleton Access**: Ensures consistent service instance management
 * 
 * @par Thread Safety
 * This class is not thread-safe. External synchronization is required
 * for concurrent access in multi-threaded environments.
 * 
 * @see IServiceVisitor
 * @see ILoginService
 * @see ServiceRegistry
 * 
 * @since 1.0
 */
class LoginServiceVisitor : public IServiceVisitor {
private:
    /**
     * @brief Cached login service instance
     * 
     * Maintains a reference to the login service obtained from ServiceRegistry.
     * This caching improves performance by avoiding repeated service lookups.
     */
    std::shared_ptr<ILoginService> _service;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes the visitor and retrieves the login service from ServiceRegistry.
     * 
     * @throws std::runtime_error If login service is not available in registry
     * 
     * @post _service contains valid login service reference
     */
    LoginServiceVisitor();

    /**
     * @brief Retrieves the cached login service instance
     * 
     * Provides access to the login service for role-specific authentication operations.
     * This method ensures that all roles interact with the same service instance.
     * 
     * @return std::shared_ptr<ILoginService> The login service instance
     * 
     * @pre LoginServiceVisitor must be properly constructed
     * @post Returns valid login service reference
     * 
     * @see ILoginService
     */
    std::shared_ptr<ILoginService> getLoginService();

    /**
     * @brief Provides login service access for Guest users
     * 
     * Enables guest users to access login functionality for initial authentication.
     * This is the primary entry point for unauthenticated users to access the system.
     * 
     * @param role The guest role requesting service access
     * 
     * @pre role must be a valid Guest instance
     * @post Guest has access to login service for authentication
     * 
     * @par Usage Example
     * @code
     * auto visitor = std::make_shared<LoginServiceVisitor>();
     * auto guest = std::make_shared<Guest>();
     * visitor->service(guest);
     * auto loginService = visitor->getLoginService();
     * @endcode
     * 
     * @see Guest
     * @see ILoginService::authenticate
     */
    void service(std::shared_ptr<Guest> role) override;

    /**
     * @brief Provides login service access for authenticated Users
     * 
     * Allows authenticated users to access login services for re-authentication
     * or session management purposes.
     * 
     * @param role The user role requesting service access
     * 
     * @pre role must be a valid User instance
     * @post User has access to login service for re-authentication
     * 
     * @see User
     * @see ILoginService::validateSession
     */
    void service(std::shared_ptr<User> role) override;

    /**
     * @brief Provides login service access for Admin users
     * 
     * Enables administrators to access login services for administrative
     * authentication and elevated privilege validation.
     * 
     * @param role The admin role requesting service access
     * 
     * @pre role must be a valid Admin instance
     * @post Admin has access to login service for administrative authentication
     * 
     * @see Admin
     * @see ILoginService::validateAdminAccess
     */
    void service(std::shared_ptr<Admin> role) override;
};

#endif