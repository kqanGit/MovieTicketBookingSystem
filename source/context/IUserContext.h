/**
 * @file IUserContext.h
 * @brief User context interface defining role-based behavior
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef IUSERCONTEXT_H
#define IUSERCONTEXT_H

#include "../model/AccountInformation.h"
#include "../visitor/IVisitor.h"
#include <memory> // For smart pointer usage

// Forward declarations
class IUserInformationService; ///< Service for user information management

/**
 * @interface IUserContext
 * @brief Interface defining behavior for different user roles in the system
 * 
 * This interface implements the Strategy pattern to provide different behaviors
 * based on user authentication status and role. It works in conjunction with
 * the Visitor pattern to enable role-specific service access.
 * 
 * @details
 * The IUserContext interface serves as:
 * - Strategy interface for role-based behavior
 * - Visitor pattern acceptor for service dispatching
 * - Context provider for user-specific operations
 * 
 * @par Design Patterns Used
 * - Strategy Pattern: Different implementations for Guest/User/Admin
 * - Visitor Pattern: Accepts visitor services for operation dispatch
 * - Context Pattern: Maintains user state and role information
 * 
 * @par Concrete Implementations
 * - GuestContext: Unauthenticated user with limited access
 * - UserContext: Authenticated regular user
 * - AdminContext: Authenticated administrator with full access
 * 
 * @see SessionManager
 * @see UserContextFactory
 * @see IVisitor
 */
class IUserContext {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     * 
     * Ensures proper destruction of derived context objects when
     * accessed through base interface pointer.
     */
    virtual ~IUserContext() = default;
    
    /**
     * @brief Accept a visitor service for operation dispatch
     * 
     * Implements the Visitor pattern acceptor role. Different context
     * implementations can route the visitor to appropriate handlers
     * based on user role and permissions.
     * 
     * @param service Visitor service to accept and process
     * 
     * @pre service != nullptr
     * 
     * @par Visitor Dispatch Logic
     * - GuestContext: Limited visitor acceptance (login/register only)
     * - UserContext: Full user-level visitor acceptance
     * - AdminContext: All visitor types including admin operations
     * 
     * @see IVisitor
     * @see LoginServiceVisitor
     * @see BookingServiceVisitor
     */
    virtual void accept(std::shared_ptr<IVisitor> service) = 0;

    /**
     * @brief Get user information service for this context
     * 
     * Returns the appropriate user information service based on the
     * current user context and authentication status.
     * 
     * @return std::shared_ptr<IUserInformationService> Service instance or nullptr
     * 
     * @retval nullptr For Guest context (no authenticated user)
     * @retval Valid service For authenticated User/Admin contexts
     * 
     * @note The returned service provides access to user profile management,
     *       booking history, and other user-specific data operations
     * 
     * @see IUserInformationService
     */
    virtual std::shared_ptr<IUserInformationService> getUserInformationService() const = 0;
};
#endif