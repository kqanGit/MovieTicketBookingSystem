/**
 * @file RegisterServiceVisitor.h
 * @brief Visitor implementation for role-based registration service access control
 * @author MovieTicketBookingSystem Team
 * @date 2025
 * 
 * This file implements the Visitor pattern for providing role-specific access
 * to user registration services within the Movie Ticket Booking System.
 */

#ifndef REGISTERSERVICEVISITOR_H
#define REGISTERSERVICEVISITOR_H
#include "IServiceVisitor.h"
#include "../core/ServiceRegistry.h"
#include "../service/IRegisterService.h"  
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

/**
 * @class RegisterServiceVisitor
 * @brief Concrete visitor for managing role-based access to registration services
 * 
 * The RegisterServiceVisitor implements the Visitor pattern to provide secure,
 * role-based access to user registration functionality. It ensures that registration
 * services are appropriately accessible based on the current user's authentication status.
 * 
 * This class enforces the business rule that:
 * - Guests can access registration services (primary use case for account creation)
 * - Users cannot access registration services (already authenticated)
 * - Admins can access registration services (for administrative user creation)
 * 
 * @par Design Pattern
 * - **Visitor Pattern**: Separates registration access logic from role objects
 * - **Service Locator**: Uses ServiceRegistry for dependency resolution
 * - **State-Based Access**: Access control based on authentication state
 * 
 * @par Security Considerations
 * This visitor implements important security controls around user registration:
 * - Prevents authenticated users from creating duplicate accounts
 * - Allows administrators to create accounts for system management
 * - Ensures proper validation and authorization workflows
 * 
 * @par Business Rules
 * - Guests must register to access personalized features
 * - Users cannot register additional accounts while authenticated
 * - Admins can create accounts for operational purposes
 * - All registrations must pass validation requirements
 * 
 * @par Thread Safety
 * This class is not thread-safe. External synchronization is required
 * for concurrent access in multi-threaded environments.
 * 
 * @see IServiceVisitor
 * @see IRegisterService
 * @see ServiceRegistry
 * 
 * @since 1.0
 */


class RegisterServiceVisitor : public IServiceVisitor{
private:
    /**
     * @brief Cached registration service instance
     * 
     * Maintains a reference to the registration service obtained from ServiceRegistry.
     * This caching improves performance by avoiding repeated service lookups during
     * registration workflows.
     */
    std::shared_ptr<IRegisterService> _service;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes the visitor and retrieves the registration service from ServiceRegistry.
     * 
     * @throws std::runtime_error If registration service is not available in registry
     * 
     * @post _service contains valid registration service reference
     */
    RegisterServiceVisitor();

    /**
     * @brief Retrieves the cached registration service instance
     * 
     * Provides access to the registration service for account creation operations.
     * This method ensures that authorized roles interact with the same service instance.
     * 
     * @return std::shared_ptr<IRegisterService> The registration service instance
     * 
     * @pre RegisterServiceVisitor must be properly constructed
     * @post Returns valid registration service reference
     * 
     * @see IRegisterService
     */
    std::shared_ptr<IRegisterService> getRegisterService();

    /**
     * @brief Provides registration service access for Guest users
     * 
     * Enables guest users to access registration services for creating new user accounts.
     * This is the primary pathway for unauthenticated users to join the system.
     * 
     * @param role The guest role requesting service access
     * 
     * @pre role must be a valid Guest instance
     * @post Guest has access to registration service for account creation
     * 
     * @par Available Registration Features
     * - Create new user accounts
     * - Validate registration information
     * - Set up initial user preferences
     * - Generate welcome communications
     * 
     * @par Usage Example
     * @code
     * auto visitor = std::make_shared<RegisterServiceVisitor>();
     * auto guest = std::make_shared<Guest>();
     * visitor->service(guest);
     * auto registerService = visitor->getRegisterService();
     * registerService->createAccount(registrationData);
     * @endcode
     * 
     * @see Guest
     * @see IRegisterService::createAccount
     */
    void service(std::shared_ptr<Guest> role) override;

    /**
     * @brief Denies registration service access for authenticated Users
     * 
     * Authenticated users are already registered and cannot access registration
     * services to prevent account duplication and maintain data integrity.
     * 
     * @param role The user role requesting service access (denied)
     * 
     * @pre role must be a valid User instance
     * @post No service access is granted to the authenticated user
     * 
     * @note This method may log the access attempt for security monitoring
     * @note Users should be directed to account management services instead
     * 
     * @see User
     * @see IUserInformationService (for account management)
     */
    void service(std::shared_ptr<User> role) override;

    /**
     * @brief Provides registration service access for Admin users
     * 
     * Enables administrators to access registration services for creating user
     * accounts in administrative contexts, such as bulk user creation or
     * system initialization procedures.
     * 
     * @param role The admin role requesting service access
     * 
     * @pre role must be a valid Admin instance
     * @pre admin must have proper authorization for user creation
     * @post Admin has access to registration service for administrative account creation
     * 
     * @par Administrative Registration Features
     * - Create user accounts with predefined roles
     * - Bulk user account creation
     * - Set administrative flags and permissions
     * - Override standard validation rules when appropriate
     * - Create test or demonstration accounts
     * 
     * @see Admin
     * @see IRegisterService::createAdminAccount
     * @see IRegisterService::bulkCreateAccounts
     */
    void service(std::shared_ptr<Admin> role) override;
};

#endif