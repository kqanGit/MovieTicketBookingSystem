/**
 * @file LoginService.h
 * @brief Authentication service implementation
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef LOGIN_SERVICE_H
#define LOGIN_SERVICE_H

#include "ILoginService.h"
#include "../repository/IAuthenticationRepository.h"
#include "../model/AccountInformation.h"
#include <optional>

/**
 * @class LoginService
 * @brief Concrete implementation of user authentication service
 * 
 * This class provides the core authentication functionality for the movie ticket
 * booking system. It implements the ILoginService interface and handles user
 * login validation through integration with authentication repositories.
 * 
 * @details
 * Key Features:
 * - Username/password authentication
 * - Repository pattern integration
 * - Optional return type for safe failure handling
 * - Support for guest and authenticated modes
 * - Account information retrieval
 * 
 * @par Design Patterns Used
 * - Strategy Pattern: Implements ILoginService interface
 * - Repository Pattern: Uses IAuthenticationRepository for data access
 * - Dependency Injection: Repository injected through constructor
 * 
 * @par Usage Example
 * @code
 * IAuthenticationRepository* authRepo = new AuthenticationRepositorySQL();
 * LoginService loginService(authRepo);
 * 
 * auto result = loginService.authenticate("john_doe", "password123");
 * if (result.has_value()) {
 *     AccountInformation account = result.value();
 *     std::cout << "Login successful for: " << account.getUsername() << std::endl;
 * }
 * @endcode
 * 
 * @warning Repository pointer must remain valid throughout service lifetime
 * 
 * @see ILoginService
 * @see IAuthenticationRepository
 * @see AccountInformation
 */
class LoginService : public ILoginService {
private:
    /**
     * @brief Authentication repository for user data access
     * 
     * Pointer to the repository responsible for user authentication data.
     * Can be nullptr for guest mode operation.
     */
    IAuthenticationRepository* repo;

public:
    /**
     * @brief Default constructor for guest mode support
     * 
     * Creates a LoginService instance without a repository.
     * Used primarily for guest user context where authentication
     * is not required.
     * 
     * @post repo == nullptr
     * 
     * @note Guest.cpp uses this constructor for unauthenticated access
     */
    LoginService() : repo(nullptr) {}
    
    /**
     * @brief Primary constructor with repository injection
     * 
     * Creates a fully functional LoginService with authentication
     * repository for user login validation.
     * 
     * @param r Pointer to authentication repository implementation
     * 
     * @pre r != nullptr for authenticated operations
     * @post repo == r
     * 
     * @par Example
     * @code
     * auto* authRepo = new AuthenticationRepositorySQL();
     * LoginService service(authRepo);
     * @endcode
     */
    LoginService(IAuthenticationRepository* r) : repo(r) {}

    /**
     * @brief Authenticate user with username and password
     * 
     * Validates user credentials against the authentication repository
     * and returns account information if successful.
     * 
     * @param username User's login username
     * @param password User's login password (plain text)
     * @return std::optional<AccountInformation> Account info if successful, empty if failed
     * 
     * @retval std::optional with AccountInformation if authentication succeeds
     * @retval std::nullopt if credentials are invalid or repository unavailable
     * 
     * @pre repo != nullptr for successful authentication
     * @pre username and password are non-empty strings
     * 
     * @post No side effects on authentication failure
     * @post Account information contains valid user data on success
     * 
     * @par Security Considerations
     * - Passwords should be hashed before storage (repository responsibility)
     * - Failed attempts should be logged for security monitoring
     * - Consider rate limiting for brute force protection
     * 
     * @warning Passwords are currently handled as plain text in this interface
     * 
     * @see IAuthenticationRepository::validateCredentials()
     * @see AccountInformation
     */
    std::optional<AccountInformation> authenticate(const std::string& username, const std::string& password);
};

#endif