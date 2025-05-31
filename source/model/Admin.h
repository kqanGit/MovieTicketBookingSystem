/**
 * @file Admin.h
 * @brief Administrator user context implementation
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef ADMIN_H
#define ADMIN_H

#include "../visitor/IVisitor.h"
#include "../visitor/IServiceVisitor.h"
#include "../context/IUserContext.h"
#include "../service/UserInformationService.h"
#include <memory> 
#include <string>

/**
 * @class Admin
 * @brief Administrator user context with elevated privileges
 * 
 * This class represents an administrator user in the movie ticket booking system.
 * Administrators have full access to system management functions including
 * movie management, user administration, booking oversight, and system configuration.
 * 
 * @details
 * Administrative Capabilities:
 * - Movie catalog management (add, edit, delete movies)
 * - Showtime scheduling and management
 * - User account administration
 * - Booking and revenue oversight
 * - System configuration and maintenance
 * - Report generation and analytics
 * 
 * @par Design Patterns Used
 * - Context Pattern: Implements IUserContext for role-based behavior
 * - Visitor Pattern: Accepts service visitors for operation delegation
 * - Strategy Pattern: Different behavior based on user type
 * - Dependency Injection: Services injected through constructor
 * 
 * @par Access Control
 * Admin users bypass most access restrictions and can perform
 * privileged operations that regular users cannot access.
 * 
 * @par Usage Example
 * @code
 * AccountInformation adminAccount("admin", UserType::ADMIN);
 * Admin adminUser(adminAccount);
 * 
 * // Accept a movie management service visitor
 * auto movieService = std::make_shared<MovieManagementService>();
 * adminUser.accept(movieService);
 * 
 * // Get user information service
 * auto infoService = adminUser.getUserInformationService();
 * @endcode
 * 
 * @warning Admin privileges should be carefully controlled and audited
 * 
 * @see IUserContext
 * @see AccountInformation
 * @see IVisitor
 * @see UserInformationService
 */
class Admin : public IUserContext {
private:
    /**
     * @brief User information service instance
     * 
     * Shared pointer to service responsible for managing user
     * account information and administrative operations.
     * Provides access to user data modification capabilities.
     */
    std::shared_ptr<UserInformationService> infoService;

public:
    /**
     * @brief Constructor with account information
     * 
     * Creates an administrator user context with the provided
     * account information and initializes administrative services.
     * 
     * @param acc Account information for the administrator
     * 
     * @pre acc.getUserType() == UserType::ADMIN
     * @pre acc contains valid administrator credentials
     * 
     * @post infoService is initialized and ready for use
     * @post Admin context is ready to accept service visitors
     * 
     * @throws std::invalid_argument if account is not admin type
     * 
     * @par Initialization Process
     * - Validates administrator privileges
     * - Initializes user information service
     * - Sets up administrative access permissions
     * - Configures audit logging for admin actions
     */
    Admin(const AccountInformation& acc);

    /**
     * @brief Accept a service visitor for operation delegation
     * 
     * Implements the Visitor pattern to allow different services
     * to operate on the admin user context. Admin users can accept
     * all types of service visitors including privileged operations.
     * 
     * @param service Shared pointer to service visitor to accept
     * 
     * @pre service != nullptr
     * @pre Admin user has necessary permissions for service operations
     * 
     * @post Service operation is executed with admin privileges
     * @post Admin actions are logged for audit purposes
     * 
     * @par Supported Services
     * Admin users can accept:
     * - Movie management services (add, edit, delete movies)
     * - User administration services
     * - Booking management services
     * - System configuration services
     * - Reporting and analytics services
     * 
     * @warning Service operations execute with elevated privileges
     * 
     * @see IVisitor
     * @see IServiceVisitor
     */
    void accept(std::shared_ptr<IVisitor> service) override;

    /**
     * @brief Get user information service
     * 
     * Returns the user information service associated with this
     * admin context, providing access to user account management
     * and administrative functions.
     * 
     * @return std::shared_ptr<IUserInformationService> Pointer to user info service
     * 
     * @retval Valid service pointer with admin privileges
     * @retval Never returns nullptr for valid admin instances
     * 
     * @post Returned service has administrative access permissions
     * 
     * @par Service Capabilities
     * The returned service provides:
     * - User account creation and modification
     * - Password reset and security management
     * - User role and permission management
     * - Account status control (enable/disable)
     * - User activity and audit trail access
     * 
     * @see IUserInformationService
     * @see UserInformationService
     */
    std::shared_ptr<IUserInformationService> getUserInformationService() const override;
};

#endif
