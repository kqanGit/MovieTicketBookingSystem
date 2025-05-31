/**
 * @file LogoutServiceVisitor.h
 * @brief Visitor implementation for role-based logout service access control
 * @author MovieTicketBookingSystem Team
 * @date 2025
 * 
 * This file implements the Visitor pattern for providing role-specific access
 * to logout services within the Movie Ticket Booking System.
 */

#ifndef LOGOUTSERVICEVISITOR_H
#define LOGOUTSERVICEVISITOR_H
#include "IServiceVisitor.h"
#include "../service/ILogoutService.h"
#include "../core/ServiceRegistry.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

/**
 * @class LogoutServiceVisitor
 * @brief Concrete visitor for managing role-based access to logout services
 * 
 * The LogoutServiceVisitor implements the Visitor pattern to provide secure,
 * role-based access to logout functionality. It ensures that logout services
 * are only accessible to authenticated users who need to terminate their sessions.
 * 
 * This class enforces the business rule that:
 * - Guests cannot access logout services (they are not authenticated)
 * - Users can access logout services to terminate their sessions
 * - Admins can access logout services for session management
 * 
 * @par Design Pattern
 * - **Visitor Pattern**: Separates logout service access logic from role objects
 * - **Service Locator**: Uses ServiceRegistry for dependency resolution
 * - **Access Control**: Enforces role-based security constraints
 * 
 * @par Security Considerations
 * This visitor ensures that only authenticated users can access logout functionality,
 * preventing unauthorized session manipulation and maintaining system security.
 * 
 * @par Thread Safety
 * This class is not thread-safe. External synchronization is required
 * for concurrent access in multi-threaded environments.
 * 
 * @see IServiceVisitor
 * @see ILogoutService
 * @see ServiceRegistry
 * 
 * @since 1.0
 */

class LogoutServiceVisitor : public IServiceVisitor {
private:
    /**
     * @brief Cached logout service instance
     * 
     * Maintains a reference to the logout service obtained from ServiceRegistry.
     * This caching improves performance by avoiding repeated service lookups.
     */
    std::shared_ptr<ILogoutService> _service;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes the visitor and retrieves the logout service from ServiceRegistry.
     * 
     * @throws std::runtime_error If logout service is not available in registry
     * 
     * @post _service contains valid logout service reference
     */
    LogoutServiceVisitor();

    /**
     * @brief Retrieves the cached logout service instance
     * 
     * Provides access to the logout service for role-specific session termination operations.
     * This method ensures that all authenticated roles interact with the same service instance.
     * 
     * @return std::shared_ptr<ILogoutService> The logout service instance
     * 
     * @pre LogoutServiceVisitor must be properly constructed
     * @post Returns valid logout service reference
     * 
     * @see ILogoutService
     */
    std::shared_ptr<ILogoutService> getLogoutService();

    /**
     * @brief Denies logout service access for Guest users
     * 
     * Guests are not authenticated and therefore cannot access logout functionality.
     * This method enforces the security constraint that prevents unauthenticated
     * users from attempting to terminate non-existent sessions.
     * 
     * @param role The guest role requesting service access (denied)
     * 
     * @pre role must be a valid Guest instance
     * @post No service access is granted to the guest
     * 
     * @note This method may log the access attempt for security monitoring
     * 
     * @see Guest
     */
    void service(std::shared_ptr<Guest> role) override;

    /**
     * @brief Provides logout service access for authenticated Users
     * 
     * Allows authenticated users to access logout services for session termination
     * and secure disconnection from the system.
     * 
     * @param role The user role requesting service access
     * 
     * @pre role must be a valid User instance
     * @pre user must have an active session
     * @post User has access to logout service for session termination
     * 
     * @par Usage Example
     * @code
     * auto visitor = std::make_shared<LogoutServiceVisitor>();
     * auto user = std::make_shared<User>();
     * visitor->service(user);
     * auto logoutService = visitor->getLogoutService();
     * logoutService->logout(user->getSessionId());
     * @endcode
     * 
     * @see User
     * @see ILogoutService::logout
     */
    void service(std::shared_ptr<User> role) override;

    /**
     * @brief Provides logout service access for Admin users
     * 
     * Enables administrators to access logout services for administrative
     * session management and secure system disconnection.
     * 
     * @param role The admin role requesting service access
     * 
     * @pre role must be a valid Admin instance
     * @pre admin must have an active session
     * @post Admin has access to logout service for session management
     * 
     * @see Admin
     * @see ILogoutService::adminLogout
     */
    void service(std::shared_ptr<Admin> role) override;
};

#endif