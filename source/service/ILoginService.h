/**
 * @file ILoginService.h
 * @brief Interface for user authentication services
 * @author Movie Ticket Booking System Team
 */

#ifndef ILOGINSERVICE_H
#define ILOGINSERVICE_H

#include <string>
#include <memory>
#include "../model/AccountInformation.h"
#include <optional>

/**
 * @interface ILoginService
 * @brief Interface defining authentication operations for the system
 * 
 * This interface provides a contract for user authentication services,
 * implementing the Strategy pattern to allow different authentication
 * mechanisms (database, LDAP, OAuth, etc.).
 * 
 * @details
 * The service uses dependency inversion principle, allowing the high-level
 * authentication logic to depend on abstractions rather than concrete implementations.
 * 
 * @see LoginService
 * @see AccountInformation
 * @since v1.0
 */
class ILoginService {
public:
    /**
     * @brief Authenticates a user with username and password
     * 
     * Validates user credentials against the configured authentication source
     * and returns account information if successful.
     * 
     * @param username The username for authentication
     * @param password The password for authentication (should be hashed in production)
     * 
     * @return std::optional<AccountInformation> containing account info if successful,
     *         std::nullopt if authentication fails
     * 
     * @pre username and password must not be empty
     * @post If successful, returns valid AccountInformation with user details
     * 
     * @note In production systems, passwords should be properly hashed and salted
     * @warning This method may block during database/network operations
     * 
     * @see AccountInformation
     * @since v1.0
     */
    virtual std::optional<AccountInformation> authenticate(const std::string& username, const std::string& password) = 0;
    
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~ILoginService() = default;
};

#endif