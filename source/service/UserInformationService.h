/**
 * @file UserInformationService.h
 * @brief Concrete implementation of user information service
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef USER_INFORMATION_SERVICE_H
#define USER_INFORMATION_SERVICE_H

#include "IUserInformationService.h"
#include "../model/AccountInformation.h"
#include <string>

/**
 * @class UserInformationService
 * @brief Concrete implementation of user information display operations
 * 
 * The UserInformationService class provides the concrete implementation for
 * displaying and accessing user account information within the movie ticket
 * booking system. This service encapsulates user data and provides controlled
 * access to account details for UI components and authorization systems.
 * 
 * @details Implementation Features:
 * - Stores and manages AccountInformation object
 * - Provides formatted output for user profile displays
 * - Implements role-based access control support
 * - Ensures secure handling of sensitive user data
 * 
 * Design Pattern: Service Pattern
 * - Implements IUserInformationService interface
 * - Encapsulates user information business logic
 * - Provides abstraction for user data access
 * - Supports dependency injection and testing
 * 
 * Security Considerations:
 * - Filters sensitive information in display methods
 * - Provides accurate role information for authorization
 * - Ensures data consistency throughout service lifetime
 * - Protects against unauthorized access to user details
 * 
 * Usage Example:
 * @code
 * AccountInformation userAccount("john_doe", "hashedPassword", "john@email.com");
 * auto userInfoService = std::make_unique<UserInformationService>(userAccount);
 * 
 * // Display user information
 * userInfoService->printAccountInfo();
 * 
 * // Check user role for authorization
 * std::string role = userInfoService->getRole();
 * if (role == "admin") {
 *     // Enable admin features
 * }
 * @endcode
 * 
 * @see IUserInformationService
 * @see AccountInformation
 * @since 1.0
 */
class UserInformationService : public IUserInformationService {
private:
    /**
     * @brief User account information managed by this service
     * 
     * AccountInformation object containing all user profile data
     * including username, email, role, and other account details.
     * This data is used for display and authorization operations.
     */
    AccountInformation accountInfo;
    
public:
    /**
     * @brief Constructs UserInformationService with account information
     * 
     * Creates a new UserInformationService instance initialized with the
     * provided user account information. The service will manage and
     * provide access to this account data throughout its lifetime.
     * 
     * @param acc AccountInformation object containing user account details
     * 
     * @pre acc should contain valid and complete account information
     * @post Service is initialized with the provided account data
     * @post Service is ready for information display and role queries
     * 
     * @note Account information is copied into the service
     * @note Changes to the original AccountInformation object won't affect the service
     * @note Service maintains its own copy of the account data
     * 
     * Usage:
     * @code
     * AccountInformation userAccount;
     * userAccount.setUsername("jane_doe");
     * userAccount.setEmail("jane@example.com");
     * userAccount.setRole("user");
     * 
     * auto userInfoService = std::make_unique<UserInformationService>(userAccount);
     * 
     * // Service is ready to use with jane_doe's information
     * userInfoService->printAccountInfo();
     * @endcode
     */
    UserInformationService(const AccountInformation& acc);
    
    /**
     * @brief Displays formatted user account information
     * 
     * Prints user account information to the standard output in a
     * formatted, user-friendly manner. This method provides a standardized
     * way to display user profile information while filtering sensitive data.
     * 
     * @post User account information is displayed to standard output
     * @post Sensitive information like passwords is not displayed
     * @post Output follows consistent formatting standards
     * 
     * @note This method is const and does not modify service state
     * @note Passwords and other sensitive data are never displayed
     * @note Output format is suitable for console interfaces
     * @note Consider implementing different output formats for different contexts
     * 
     * Security Features:
     * - Never displays password or other sensitive authentication data
     * - May mask or truncate sensitive fields like email addresses
     * - Provides only information appropriate for the current context
     * - Follows privacy best practices for user data display
     * 
     * Implementation Details:
     * - Accesses stored AccountInformation object
     * - Formats output in user-friendly manner
     * - Filters out sensitive information automatically
     * - Uses consistent formatting across all user displays
     * 
     * Example Output:
     * @code
     * // Example of what might be displayed:
     * =====================================
     * User Account Information
     * =====================================
     * Username: john_doe
     * Email: j***@example.com
     * Role: user
     * Account Status: Active
     * Member Since: 2024-01-15
     * =====================================
     * @endcode
     * 
     * Usage:
     * @code
     * // Display current user information in console
     * std::cout << "Current User Profile:" << std::endl;
     * userInfoService->printAccountInfo();
     * 
     * // Can be used in menu systems
     * std::cout << "\n1. View Profile" << std::endl;
     * if (choice == 1) {
     *     userInfoService->printAccountInfo();
     * }
     * @endcode
     */
    void printAccountInfo() const;
    
    /**
     * @brief Retrieves the user's role for authorization purposes
     * 
     * Returns the role or permission level of the user associated with
     * this service. This method is used throughout the system for
     * authorization decisions and access control enforcement.
     * 
     * @return String representation of the user's role (e.g., "user", "admin", "manager")
     * 
     * @post Returns accurate, current role information from stored account data
     * @post Role string matches system-wide role definitions
     * 
     * @note This method is const and does not modify service state
     * @note Role information comes from the stored AccountInformation
     * @note Role strings should be consistent with system authorization logic
     * @note Consider using enums for type safety in future versions
     * 
     * Role Definitions:
     * - "guest": Anonymous user with limited access (shouldn't apply here)
     * - "user": Registered user with standard booking permissions
     * - "admin": Administrator with full system access
     * - "manager": Cinema manager with operational permissions
     * 
     * Implementation Details:
     * - Retrieves role from stored AccountInformation object
     * - Returns role string as stored in account data
     * - No additional processing or validation performed
     * - Role accuracy depends on proper account initialization
     * 
     * Usage:
     * @code
     * std::string userRole = userInfoService->getRole();
     * 
     * // Authorization logic
     * if (userRole == "admin") {
     *     // Enable administrative features
     *     showAdminPanel();
     *     enableMovieManagement();
     *     enableUserManagement();
     * } else if (userRole == "manager") {
     *     // Enable manager features
     *     showManagerPanel();
     *     enableMovieManagement();
     * } else if (userRole == "user") {
     *     // Standard user features
     *     showUserDashboard();
     *     enableBookingFeatures();
     * }
     * 
     * // Menu customization based on role
     * if (userRole == "admin" || userRole == "manager") {
     *     addMenuItem("Manage Movies");
     * }
     * @endcode
     */
    std::string getRole() const override;
};

#endif
