#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <memory>
#include <string>
#include "IUserContext.h"
#include "AccountInformation.h"
#include "../repository/IAuthenticationRepository.h"
#include "UserContextFactory.h"

/**
 * @class SessionManager
 * @brief Manages user sessions and context transitions
 * 
 * The SessionManager is responsible for managing user sessions,
 * including keeping track of the current user context and providing
 * methods for transitions between different user states (Guest, User, Admin).
 * It centralizes user context management to provide a clearer API.
 */
class SessionManager {
private:
    std::unique_ptr<IUserContext> currentContext;
    IAuthenticationRepository* authRepo;
    std::unique_ptr<UserContextFactory> guestFactory;
    std::unique_ptr<UserContextFactory> userFactory;
    std::unique_ptr<UserContextFactory> adminFactory;
    
    // Current session information
    AccountInformation currentAccount;
    bool isAuthenticated;

public:
    /**
     * @brief Constructs a SessionManager with a repository and factories
     * @param repo Authentication repository
     */
    SessionManager(IAuthenticationRepository* repo);
    
    /**
     * @brief Get the current user context
     * @return Pointer to the current user context
     */
    IUserContext* getCurrentContext() const;

    /**
     * @brief Get the role of the current user context
     * @return String representing the role (guest, user, admin)
     */
    std::string getCurrentRole() const;
    
    /**
     * @brief Check if user is authenticated
     * @return True if authenticated, false otherwise
     */
    bool isUserAuthenticated() const;
    
    /**
     * @brief Get current account information
     * @return Account information of the current user
     */
    const AccountInformation& getCurrentAccount() const;
    
    /**
     * @brief Login with username and password
     * @param username Username to authenticate
     * @param password Password for authentication
     * @return True if login successful, false otherwise
     */
    bool login(const std::string& username, const std::string& password);
    
    /**
     * @brief Logout current user
     * @return True if logout successful, false otherwise
     */
    bool logout();
    
    /**
     * @brief Register a new user
     * @param info Account information for registration
     * @return True if registration successful, false otherwise
     */
    bool registerUser(const AccountInformation& info);
};

#endif // SESSION_MANAGER_H
