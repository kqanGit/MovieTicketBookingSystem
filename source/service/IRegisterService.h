/**
 * @file IRegisterService.h
 * @brief User registration service interface definition
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef IREGISTER_SERVICE_H
#define IREGISTER_SERVICE_H

#include <string>
#include "../model/AccountInformation.h"

/**
 * @interface IRegisterService
 * @brief Interface for user registration operations
 * 
 * The IRegisterService interface defines the contract for user registration
 * functionality in the movie ticket booking system. This interface follows
 * the Service pattern and Separation of Concerns principle by focusing
 * solely on registration logic without handling context creation.
 * 
 * @details Key Responsibilities:
 * - Validate user registration information
 * - Create new user accounts in the system
 * - Ensure data integrity and uniqueness constraints
 * - Handle registration business rules
 * 
 * Design Pattern: Service Pattern
 * - Encapsulates registration business logic
 * - Provides abstraction for different registration implementations
 * - Enables dependency injection and unit testing
 * - Separates registration concerns from authentication and context management
 * 
 * Separation of Concerns:
 * - Registration service only handles account creation
 * - Does not manage user sessions or contexts
 * - Authentication and context creation are handled by separate services
 * - Focused responsibility improves maintainability and testability
 * 
 * Security Considerations:
 * - Password validation and hashing should be implemented
 * - Input sanitization should be performed
 * - Rate limiting may be applied to prevent abuse
 * - Audit logging should track registration attempts
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<IRegisterService> registerService = 
 *     std::make_unique<RegisterService>(authRepo);
 * 
 * AccountInformation newUser("john.doe", "securePassword123", "john@email.com");
 * bool success = registerService->registerUser(newUser);
 * 
 * if (success) {
 *     // Registration successful, proceed with login
 * } else {
 *     // Handle registration failure
 * }
 * @endcode
 * 
 * @see RegisterService
 * @see AccountInformation
 * @see IAuthenticationRepository
 * @since 1.0
 */
class IRegisterService {
public:
    /**
     * @brief Registers a new user account in the system
     * 
     * Creates a new user account using the provided account information.
     * This method validates the input data, ensures uniqueness constraints,
     * and persists the new account to the system. It follows the principle
     * of Separation of Concerns by only handling registration logic.
     * 
     * @param info Account information containing user details (username, password, email, etc.)
     * @return true if registration is successful, false otherwise
     * 
     * @pre info must contain valid account information
     * @pre username must not already exist in the system
     * @pre email must not already be registered (if email uniqueness is required)
     * @pre password must meet system security requirements
     * @post New user account is created and persisted if successful
     * @post User can subsequently authenticate with the provided credentials
     * 
     * @throw std::invalid_argument if account information is invalid
     * @throw DuplicateUserException if username or email already exists
     * @throw WeakPasswordException if password doesn't meet security requirements
     * @throw std::runtime_error if database operation fails
     * 
     * @note This method does not create user contexts or handle authentication
     * @note Password should be hashed before storage (handled by implementation)
     * @note Consider implementing email verification workflow
     * @note Return value indicates success/failure; exceptions for specific errors
     * 
     * Business Rules:
     * - Username must be unique across the system
     * - Password must meet minimum security standards
     * - Email format must be valid
     * - All required fields must be provided
     * 
     * Usage:
     * @code
     * AccountInformation userInfo;
     * userInfo.setUsername("newuser");
     * userInfo.setPassword("StrongPassword123!");
     * userInfo.setEmail("user@example.com");
     * 
     * try {
     *     bool success = registerService->registerUser(userInfo);
     *     if (success) {
     *         std::cout << "Registration successful!" << std::endl;
     *         // Redirect to login page
     *     } else {
     *         std::cout << "Registration failed. Please try again." << std::endl;
     *     }
     * } catch (const DuplicateUserException& e) {
     *     std::cout << "Username already exists." << std::endl;
     * } catch (const WeakPasswordException& e) {
     *     std::cout << "Password does not meet requirements." << std::endl;
     * }
     * @endcode
     */
    virtual bool registerUser(const AccountInformation& info) = 0;
    
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     * 
     * Ensures that concrete register service implementations are properly
     * destroyed when accessed through the interface pointer.
     */
    virtual ~IRegisterService() = default;
};

#endif
