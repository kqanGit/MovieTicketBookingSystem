#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <memory>
#include <string>
#include "IUserContext.h"
#include "AccountInformation.h"
#include "UserContextFactory.h"

/**
 * @class SessionManager
 * @brief Manages user sessions and context transitions
 * 
 * The SessionManager is responsible for managing user sessions,
 * including keeping track of the current user context and providing
 * methods for transitions between different user states (Guest, User, Admin).
 * It centralizes user context management to provide a clearer API.
 * 
 * The SessionManager ONLY manages the context state and transitions,
 * it does not perform business logic like authentication or registration.
 */
class SessionManager {
private:
    // Con trỏ đa hình quản lý context hiện tại (Guest/User/Admin), đảm bảo đúng OOP/SOLID
    std::unique_ptr<IUserContext> currentContext;
    std::unique_ptr<UserContextFactory> guestFactory;
    std::unique_ptr<UserContextFactory> userFactory;
    std::unique_ptr<UserContextFactory> adminFactory;
    
    // Current session information
    AccountInformation currentAccount;
    bool isAuthenticated;

public:
    /**
     * @brief Constructs a SessionManager with default Guest context
     */
    SessionManager();
    
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
     * @brief Set user context after successful authentication
     * @param authInfo Account information from successful authentication
     * @return True if context was set, false otherwise (e.g. already logged in)
     */
    bool setUserContext(const AccountInformation& authInfo);
    
    /**
     * @brief Logout current user and reset to Guest context
     * @return True if logout successful, false otherwise
     */
    bool logout();
};

#endif // SESSION_MANAGER_H
