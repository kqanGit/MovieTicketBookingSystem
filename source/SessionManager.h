/**
 * @file SessionManager.h
 * @brief Session management and user context handling
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <memory>
#include <string>
#include "context/IUserContext.h"
#include "model/AccountInformation.h"
#include "context/UserContextFactory.h"
#include "context/GuestContextCreator.h"
#include "context/UserContextCreator.h"
#include "context/AdminContextCreator.h"

/**
 * @class SessionManager
 * @brief Manages user sessions and context transitions in the movie booking system
 * 
 * The SessionManager class is responsible for maintaining the current user's
 * authentication state and managing transitions between different user contexts
 * (Guest, User, Admin). It implements a state machine pattern for user roles.
 * 
 * @details
 * Key responsibilities:
 * - Manage current user authentication state
 * - Handle context switching between Guest/User/Admin roles
 * - Maintain session information and account details
 * - Coordinate with the Factory pattern for context creation
 * 
 * @par Design Patterns Used
 * - State Pattern: Different behaviors based on user role state
 * - Factory Pattern: Delegates context creation to UserContextFactory
 * - Singleton-like behavior: Typically one session manager per application
 * 
 * @par Thread Safety
 * This class is not inherently thread-safe. External synchronization
 * is required if accessed from multiple threads.
 * 
 * @see IUserContext
 * @see UserContextFactory
 * @see AccountInformation
 */
class SessionManager {
private:
    /**
     * @brief Current user context pointer (Guest/User/Admin)
     * 
     * Maintains the active user context which determines available
     * operations and UI elements. Context changes based on authentication state.
     * 
     * @invariant _currentUserContext is never null
     */
    std::unique_ptr<IUserContext> _currentUserContext;
    
    /**
     * @brief Factory for creating different user context types
     * 
     * Shared factory instance used to create appropriate context objects
     * based on user authentication and role information.
     */
    std::shared_ptr<UserContextFactory> _contextFactory;
    
    /**
     * @brief Current account information
     * 
     * Stores detailed information about the currently authenticated user
     * including username, role, and other profile data.
     */
    AccountInformation _currentAccount;
    
    /**
     * @brief Authentication status flag
     * 
     * Tracks whether the current session represents an authenticated user
     * or a guest session.
     */
    bool _isAuthenticated;
    
public:
    /**
     * @brief Default constructor - initializes with Guest context
     * 
     * Creates a new session manager with default Guest context,
     * sets up the context factory, and initializes authentication state to false.
     * 
     * @post _isAuthenticated == false
     * @post getCurrentRole() == "guest"
     * @post _currentUserContext != nullptr
     */
    SessionManager();
    
    /**
     * @brief Get the current user context
     * 
     * @return IUserContext* Pointer to the current user context (never null)
     * 
     * @note The returned pointer is valid until the next context change
     * @see setUserContext()
     * @see logout()
     */
    IUserContext* getCurrentContext() const;
    
    /**
     * @brief Get the current user role as string
     * 
     * @return std::string Role identifier ("guest", "user", "admin")
     * 
     * @retval "guest" User is not authenticated
     * @retval "user" User is authenticated as regular user
     * @retval "admin" User is authenticated as administrator
     */
    std::string getCurrentRole() const;
    
    /**
     * @brief Check if user is currently authenticated
     * 
     * @return bool True if user is logged in, false for guest sessions
     * 
     * @note Authentication state is independent of user role
     */    /**
     * @brief Check if user is currently authenticated
     * 
     * @return bool True if user is logged in, false for guest sessions
     * 
     * @note Authentication state is independent of user role
     */
    bool isUserAuthenticated() const;
    
    /**
     * @brief Get current account information
     * 
     * @return const AccountInformation& Reference to current account details
     * 
     * @warning Only valid when isUserAuthenticated() returns true
     * @throws std::runtime_error If called when not authenticated
     * 
     * @see isUserAuthenticated()
     */
    const AccountInformation& getCurrentAccount() const;
    
    /**
     * @brief Set user context after successful authentication
     * 
     * Transitions from Guest context to appropriate User/Admin context
     * based on the provided authentication information.
     * 
     * @param authInfo Account information from successful authentication
     * @return bool True if context was set successfully, false if already authenticated
     * 
     * @pre isUserAuthenticated() == false (for new login)
     * @post isUserAuthenticated() == true (on success)
     * @post getCurrentAccount() == authInfo (on success)
     * 
     * @par Context Selection Logic
     * - If authInfo.role == "admin" → AdminContext
     * - If authInfo.role == "user" → UserContext
     * - Otherwise → remains GuestContext
     * 
     * @see logout()
     * @see UserContextFactory::createContext()
     */
    bool setUserContext(const AccountInformation& authInfo);
    
    /**
     * @brief Logout current user and reset to Guest context
     * 
     * Performs cleanup of current session, clears authentication state,
     * and transitions back to Guest context.
     * 
     * @return bool True if logout was successful, false if not authenticated
     * 
     * @pre isUserAuthenticated() == true
     * @post isUserAuthenticated() == false
     * @post getCurrentRole() == "guest"
     * 
     * @par Logout Process
     * 1. Clear current account information
     * 2. Set authentication flag to false
     * 3. Create new Guest context
     * 4. Update internal state
     * 
     * @see setUserContext()
     */
    bool logout();
};

#endif // SESSION_MANAGER_H