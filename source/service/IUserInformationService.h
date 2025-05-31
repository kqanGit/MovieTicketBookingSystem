/**
 * @file IUserInformationService.h
 * @brief User information service interface definition
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef IUSERINFORMATIONSERVICE_H
#define IUSERINFORMATIONSERVICE_H

#include "../model/AccountInformation.h"

/**
 * @interface IUserInformationService
 * @brief Interface for user information display and management operations
 * 
 * The IUserInformationService interface defines the contract for accessing
 * and displaying user account information within the movie ticket booking
 * system. This interface provides read-only access to user data and is
 * typically used for profile viewing and user interface display purposes.
 * 
 * @details Key Responsibilities:
 * - Display user account information
 * - Provide user role information for authorization
 * - Abstract user data access for UI components
 * - Support user profile viewing functionality
 * 
 * Design Pattern: Service Pattern
 * - Encapsulates user information business logic
 * - Provides abstraction for different information display implementations
 * - Enables clean separation between data access and presentation
 * - Supports dependency injection and testing
 * 
 * Usage Context:
 * - User profile pages and account settings
 * - Authorization and role-based access control
 * - User information display in navigation bars
 * - Account verification and validation workflows
 * 
 * Security Considerations:
 * - Sensitive information should be filtered based on context
 * - Access control should be enforced at the service level
 * - Personal data should be handled according to privacy policies
 * - Role information should be accurate for security decisions
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<IUserInformationService> userInfoService = 
 *     std::make_unique<UserInformationService>(accountInfo);
 * 
 * // Display user account information
 * userInfoService->printAccountInfo();
 * 
 * // Check user role for authorization
 * std::string role = userInfoService->getRole();
 * if (role == "admin") {
 *     // Grant administrative access
 * }
 * @endcode
 * 
 * @see UserInformationService
 * @see AccountInformation
 * @since 1.0
 */
class IUserInformationService {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     * 
     * Ensures that concrete user information service implementations are properly
     * destroyed when accessed through the interface pointer.
     */
    virtual ~IUserInformationService() = default;
    
    /**
     * @brief Displays user account information to output stream
     * 
     * Prints formatted user account information to the standard output or
     * configured output stream. This method provides a standardized way
     * to display user profile information for console interfaces or logging.
     * 
     * @post User account information is formatted and displayed
     * @post Sensitive information is appropriately filtered
     * @post Output follows consistent formatting standards
     * 
     * @note This method is const as it does not modify the service state
     * @note Sensitive information like passwords should never be displayed
     * @note Output format should be user-friendly and readable
     * @note Consider localization for international users
     * 
     * Security Note:
     * - Never display sensitive information like passwords
     * - Consider masking or truncating sensitive fields like email
     * - Ensure output is appropriate for the current security context
     * 
     * Usage:
     * @code
     * // Display user information in console interface
     * std::cout << "Current User Information:" << std::endl;
     * userInfoService->printAccountInfo();
     * 
     * // Example output:
     * // Username: john_doe
     * // Email: j***@example.com
     * // Role: user
     * // Member Since: 2024-01-15
     * @endcode
     */
    virtual void printAccountInfo() const = 0;
    
    /**
     * @brief Retrieves the user's role for authorization purposes
     * 
     * Returns the role or permission level of the current user, which is
     * used for authorization decisions and access control throughout the
     * system. Common roles include "guest", "user", "admin", "manager".
     * 
     * @return String representation of the user's role
     * 
     * @post Returns accurate, current role information
     * @post Role string is consistent with system role definitions
     * 
     * @note This method is const as it does not modify the service state
     * @note Role information should be current and not cached if dynamic
     * @note Role strings should follow system-wide naming conventions
     * @note Consider using enums instead of strings for type safety
     * 
     * Common Role Values:
     * - "guest": Anonymous user with limited access
     * - "user": Registered user with standard permissions
     * - "admin": Administrator with full system access
     * - "manager": Cinema manager with operational permissions
     * 
     * Usage:
     * @code
     * std::string userRole = userInfoService->getRole();
     * 
     * // Role-based authorization
     * if (userRole == "admin") {
     *     // Enable administrative features
     *     showAdminPanel();
     * } else if (userRole == "user") {
     *     // Show standard user interface
     *     showUserDashboard();
     * } else {
     *     // Limit to guest functionality
     *     showGuestInterface();
     * }
     * @endcode
     */
    virtual std::string getRole() const = 0;
};

#endif // IUSERINFORMATIONSERVICE_H