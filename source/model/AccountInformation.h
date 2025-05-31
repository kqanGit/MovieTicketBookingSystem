/**
 * @file AccountInformation.h
 * @brief User account information data structure
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef ACCOUNT_INFORMATION_H
#define ACCOUNT_INFORMATION_H

#include <string>

/**
 * @struct AccountInformation
 * @brief Data structure containing user account information and credentials
 * 
 * This structure serves as a Data Transfer Object (DTO) that encapsulates
 * all essential user account information used throughout the application.
 * It provides a standardized way to pass user data between different
 * layers of the system.
 * 
 * @details
 * Key Features:
 * - Stores user identification and authentication data
 * - Contains contact and profile information
 * - Role-based access control support
 * - Simple POD (Plain Old Data) structure for easy serialization
 * 
 * Design Patterns:
 * - **Data Transfer Object (DTO)**: Encapsulates data for transfer between layers
 * - **Value Object**: Immutable data container for user information
 * 
 * Usage Context:
 * - User authentication and login processes
 * - Profile management and updates
 * - Role-based access control decisions
 * - User context creation and session management
 * 
 * @par Usage Example
 * @code
 * AccountInformation userInfo;
 * userInfo.userID = 123;
 * userInfo.userName = "john_doe";
 * userInfo.password = "hashed_password";
 * userInfo.phoneNumber = "+1-555-0123";
 * userInfo.gmail = "john@example.com";
 * userInfo.role = "USER";
 * 
 * // Pass to authentication service
 * bool isValid = authService->validateUser(userInfo);
 * @endcode
 * 
 * @note Password should always be stored in hashed format for security
 * @warning Ensure sensitive data is properly secured in memory
 * 
 * @see ILoginService
 * @see UserContextFactory
 * @see IAuthenticationRepository
 */
struct AccountInformation {
    /**
     * @brief Unique user identifier
     * 
     * Database primary key that uniquely identifies the user in the system.
     * Used for foreign key relationships and user lookups.
     * 
     * @note Value of 0 indicates uninitialized or invalid user
     */
    int userID;
    
    /**
     * @brief User login name
     * 
     * Unique username used for authentication and login purposes.
     * Must be unique across the entire system.
     * 
     * @pre Must be non-empty and unique in the database
     * @note Case-sensitive username validation
     */
    std::string userName;
    
    /**
     * @brief User password (should be hashed)
     * 
     * User's authentication password. Should always be stored
     * in hashed format for security purposes.
     * 
     * @warning Never store plain text passwords
     * @note Use strong hashing algorithms (e.g., bcrypt, Argon2)
     */
    std::string password;
    
    /**
     * @brief User contact phone number
     * 
     * Phone number for user contact and verification purposes.
     * Can be used for SMS notifications and account recovery.
     * 
     * @note Should include country code for international support
     */
    std::string phoneNumber;
    
    /**
     * @brief User email address
     * 
     * Primary email address for communication, notifications,
     * and account recovery purposes.
     * 
     * @pre Must be a valid email format
     * @note Used for booking confirmations and system notifications
     */
    std::string gmail;
    
    /**
     * @brief User role for access control
     * 
     * Defines the user's role in the system for role-based access control.
     * Common values: "USER", "ADMIN", "GUEST"
     * 
     * @note Determines available features and permissions
     * @see UserContextFactory for role-based context creation
     */
    std::string role;

    /**
     * @brief Default constructor
     * 
     * Initializes the account information with default values.
     * Sets userID to 0 to indicate an uninitialized account.
     * 
     * @post userID == 0 (invalid/uninitialized state)
     * @post All string fields are empty
     * 
     * @note Requires subsequent initialization before use
     */
    AccountInformation() : userID(0) {}
    
    /**
     * @brief Parameterized constructor
     * 
     * Initializes account information with provided values.
     * Useful for creating complete account objects in one step.
     * 
     * @param id User identifier
     * @param username Login username
     * @param pass User password (should be hashed)
     * @param phone Contact phone number
     * @param email Email address
     * @param userRole User role for access control
     * 
     * @pre id > 0 for valid users
     * @pre username is non-empty and unique
     * @pre email is valid format
     * @post All fields are initialized with provided values
     */
    AccountInformation(int id, const std::string& username, const std::string& pass,
                      const std::string& phone, const std::string& email, const std::string& userRole)
        : userID(id), userName(username), password(pass), phoneNumber(phone), gmail(email), role(userRole) {}
};

#endif // ACCOUNT_INFORMATION_H