/**
 * @file RegisterService.h
 * @brief Concrete implementation of user registration service
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef REGISTER_SERVICE_H
#define REGISTER_SERVICE_H

#include "IRegisterService.h"
#include "../repository/IAuthenticationRepository.h"

/**
 * @class RegisterService
 * @brief Concrete implementation of user registration operations
 * 
 * The RegisterService class provides the concrete implementation of user
 * registration functionality for the movie ticket booking system. This
 * service handles account creation while following the Separation of
 * Concerns principle by focusing solely on registration logic.
 * 
 * @details Implementation Features:
 * - Integrates with IAuthenticationRepository for data persistence
 * - Validates user input according to business rules
 * - Ensures username and email uniqueness
 * - Handles password security requirements
 * - Provides comprehensive error handling
 * 
 * Design Pattern: Service Pattern
 * - Implements IRegisterService interface
 * - Uses Repository pattern for data access
 * - Follows Separation of Concerns principle
 * - Enables dependency injection for testing
 * 
 * Security Features:
 * - Input validation and sanitization
 * - Password strength enforcement
 * - Duplicate account prevention
 * - Secure data handling practices
 * 
 * Usage Example:
 * @code
 * auto authRepo = std::make_unique<AuthenticationRepositorySQL>(dbConnection);
 * auto registerService = std::make_unique<RegisterService>(authRepo.get());
 * 
 * AccountInformation newUser("username", "password", "email@example.com");
 * bool success = registerService->registerUser(newUser);
 * @endcode
 * 
 * @see IRegisterService
 * @see IAuthenticationRepository
 * @see AccountInformation
 * @since 1.0
 */
class RegisterService : public IRegisterService {
private:
    /**
     * @brief Repository for authentication data access operations
     * 
     * Raw pointer to the authentication repository used for user
     * account persistence. The repository handles all database
     * operations related to user account management.
     */
    IAuthenticationRepository* repo;
    
public:
    /**
     * @brief Default constructor for RegisterService
     * 
     * Creates a RegisterService instance with no repository connection.
     * The repository must be set before the service can be used for
     * registration operations.
     * 
     * @post Service is created with null repository
     * @post Repository must be set before use
     * 
     * @note This constructor is provided for scenarios where repository
     *       injection happens after construction
     */
    RegisterService() : repo(nullptr) {} // Default constructor
    
    /**
     * @brief Constructs RegisterService with repository dependency
     * 
     * Creates a RegisterService instance with the provided authentication
     * repository for data access operations. The repository must remain
     * valid for the lifetime of the service.
     * 
     * @param r Pointer to authentication repository implementation
     * 
     * @pre r should be a valid, initialized repository (null is allowed but service won't function)
     * @post Service is ready for registration operations if repository is valid
     * 
     * @note Repository pointer is stored as-is; caller retains ownership
     * @note Service will not function properly if repository is null
     * 
     * Usage:
     * @code
     * auto authRepo = std::make_unique<AuthenticationRepositorySQL>(connection);
     * auto registerService = std::make_unique<RegisterService>(authRepo.get());
     * 
     * // Service is ready for user registration
     * bool success = registerService->registerUser(accountInfo);
     * @endcode
     */
    RegisterService(IAuthenticationRepository* r) : repo(r) {} // Parameterized constructor
    
    /**
     * @brief Registers a new user account in the system
     * 
     * Creates a new user account using the provided account information.
     * This method validates all input data, enforces business rules,
     * checks for duplicate accounts, and persists the new user to the database.
     * 
     * @param info Account information containing user registration details
     * @return true if registration succeeds, false if it fails
     * 
     * @pre info must contain valid and complete account information
     * @pre Repository must be properly initialized and connected
     * @pre Username must not already exist in the system
     * @pre Email must not already be registered (if email uniqueness required)
     * @pre Password must meet system security requirements
     * @post New user account is created and persisted if successful
     * @post User can authenticate with provided credentials after successful registration
     * 
     * @throw std::invalid_argument if account information is invalid or incomplete
     * @throw std::runtime_error if repository is null or database operation fails
     * @throw DuplicateUserException if username or email already exists
     * @throw WeakPasswordException if password doesn't meet requirements
     * 
     * @note This method only handles registration; context creation is separate
     * @note Password hashing should be handled by the repository layer
     * @note Input validation includes format checking and business rule enforcement
     * @note Transactional behavior depends on repository implementation
     * 
     * Validation Rules Applied:
     * - Username must be non-empty and follow naming conventions
     * - Password must meet minimum security standards
     * - Email must be in valid format (if provided)
     * - All required fields must be populated
     * - Username uniqueness across the system
     * 
     * Implementation Details:
     * - Validates repository availability before proceeding
     * - Performs comprehensive input validation
     * - Checks for existing users with same username/email
     * - Delegates actual persistence to repository
     * - Provides detailed error information for failures
     * 
     * Usage:
     * @code
     * AccountInformation userInfo;
     * userInfo.setUsername("newuser123");
     * userInfo.setPassword("SecurePassword!@#");
     * userInfo.setEmail("user@example.com");
     * userInfo.setRole("user");
     * 
     * try {
     *     bool success = registerService->registerUser(userInfo);
     *     
     *     if (success) {
     *         std::cout << "Registration successful! Please log in." << std::endl;
     *         // Redirect to login page
     *     } else {
     *         std::cout << "Registration failed. Please check your information." << std::endl;
     *     }
     * } catch (const DuplicateUserException& e) {
     *     std::cout << "Username already exists. Please choose another." << std::endl;
     * } catch (const WeakPasswordException& e) {
     *     std::cout << "Password must be stronger. " << e.what() << std::endl;
     * } catch (const std::exception& e) {
     *     std::cout << "Registration error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    bool registerUser(const AccountInformation& info) override;
};

#endif
