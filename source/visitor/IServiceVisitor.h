

/**
 * @file IServiceVisitor.h
 * @brief Service visitor interface for role-based operations
 * @author Movie Ticket Booking System Team
 */

#ifndef ISERVICEVISITOR_H
#define ISERVICEVISITOR_H
#include "IVisitor.h"
#include <memory>

// Forward declarations
class Guest;
class User;
class Admin;

/**
 * @interface IServiceVisitor
 * @brief Visitor interface for role-based service operations
 * 
 * This interface extends IVisitor to provide role-specific service operations,
 * implementing the Visitor pattern with double dispatch for user context handling.
 * Each service operation is customized based on the user's role and permissions.
 * 
 * @details
 * **Design Pattern Implementation:**
 * - **Visitor Pattern**: Enables operations on user contexts without modifying them
 * - **Double Dispatch**: Method selection based on both visitor and user role types
 * - **Strategy Pattern**: Different behaviors for different user roles
 * 
 * **Role-Based Access Control:**
 * - **Guest**: Limited access to public information and registration
 * - **User**: Standard user operations like booking and account management
 * - **Admin**: Administrative functions like movie management and reporting
 * 
 * **Benefits:**
 * - Clean separation between user roles and service logic
 * - Easy to add new services without modifying user classes
 * - Type-safe role-based operation dispatch
 * - Maintainable and extensible service architecture
 * 
 * @see IVisitor
 * @see Guest
 * @see User
 * @see Admin
 * @see LoginServiceVisitor
 * @see BookingServiceVisitor
 * @since v1.0
 */
class IServiceVisitor : public IVisitor {
public:
    /**
     * @brief Provides service operations for Guest users
     * 
     * Handles service requests from guest users with appropriate
     * permission restrictions and limited functionality access.
     * 
     * @param role Shared pointer to the Guest user context
     * 
     * @pre role must be a valid Guest instance
     * @post Service operation is performed with guest-level permissions
     * 
     * @note Guest operations typically include: movie browsing, registration
     * @warning Access control must be enforced for guest users
     * 
     * @see Guest
     * @since v1.0
     */
    virtual void service(std::shared_ptr<Guest> role) = 0;
    
    /**
     * @brief Provides service operations for registered Users
     * 
     * Handles service requests from authenticated users with
     * standard user permissions and full booking capabilities.
     * 
     * @param role Shared pointer to the User context
     * 
     * @pre role must be a valid User instance
     * @post Service operation is performed with user-level permissions
     * 
     * @note User operations include: booking, account management, history viewing
     * 
     * @see User
     * @since v1.0
     */
    virtual void service(std::shared_ptr<User> role) = 0;
    
    /**
     * @brief Provides service operations for Admin users
     * 
     * Handles service requests from administrators with
     * elevated permissions and management capabilities.
     * 
     * @param role Shared pointer to the Admin context
     * 
     * @pre role must be a valid Admin instance
     * @post Service operation is performed with admin-level permissions
     * 
     * @note Admin operations include: movie management, user administration, reporting
     * @warning Admin operations must implement proper authorization checks
     * 
     * @see Admin
     * @since v1.0
     */
    virtual void service(std::shared_ptr<Admin> role) = 0;
};

#endif 