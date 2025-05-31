/**
 * @file IAuthenticationRepository.h
 * @brief Interface for user authentication data access operations
 * @author Movie Ticket Booking System Team
 */

#ifndef IAUTHENTICATIONREPOSITORY_H
#define IAUTHENTICATIONREPOSITORY_H

#include <string>
#include "../model/AccountInformation.h" // Corrected path
#include "../model/AccountInformation.h" // Corrected path

/**
 * @interface IAuthenticationRepository
 * @brief Repository interface for user authentication data persistence
 * 
 * This interface defines the contract for accessing and managing user
 * authentication data, implementing the Repository pattern to abstract
 * data storage implementation details.
 * 
 * @details
 * The repository provides:
 * - User account creation and storage
 * - Credential verification and user retrieval
 * - Abstraction over data storage mechanisms
 * - Support for different persistence backends
 * 
 * Design patterns implemented:
 * - **Repository Pattern**: Encapsulates data access logic
 * - **Dependency Inversion**: High-level code depends on abstractions
 * - **Strategy Pattern**: Allows different storage implementations
 * 
 * @see AuthenticationRepositorySQL
 * @see AccountInformation
 * @since v1.0
 */
class IAuthenticationRepository {
public:
    /**
     * @brief Adds a new user account to the repository
     * 
     * Creates a new user account with the provided information,
     * typically during user registration process.
     * 
     * @param account The account information to store
     * 
     * @pre account must contain valid user information
     * @pre username must be unique in the system
     * @pre password should be properly hashed
     * @post New user account is persisted in storage
     * 
     * @throws std::invalid_argument if account data is invalid
     * @throws std::runtime_error if username already exists
     * @throws std::runtime_error if storage operation fails
     * 
     * @note This operation should be atomic to prevent partial user creation
     * @warning In production, ensure password is hashed before storage
     * 
     * @see AccountInformation
     * @since v1.0
     */
    virtual void addUser(const AccountInformation& account) = 0;
    
    /**
     * @brief Retrieves user account by username and password
     * 
     * Validates user credentials and returns account information
     * if authentication is successful.
     * 
     * @param username The username for authentication
     * @param password The password for authentication
     * 
     * @return AccountInformation for the authenticated user
     * 
     * @pre username and password must not be empty
     * @post Returns valid account information if credentials match
     * 
     * @throws std::invalid_argument if username or password is empty
     * @throws std::runtime_error if user not found or password invalid
     * @throws std::runtime_error if storage access fails
     * 
     * @note This method should implement proper security measures
     * @warning Consider rate limiting to prevent brute force attacks
     * 
     * @see AccountInformation
     * @since v1.0
     */
    virtual AccountInformation getUserByUserName(const std::string& username, const std::string& password) = 0;
    
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~IAuthenticationRepository() = default;
};

#endif
