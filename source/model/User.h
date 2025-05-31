
/**
 * @file User.h
 * @brief Regular user implementation with standard privileges
 * @author Movie Ticket Booking System Team
 */

#ifndef USER_H
#define USER_H
#include "../visitor/IVisitor.h"
#include "../visitor/IServiceVisitor.h"
#include "../context/IUserContext.h"
#include "../service/UserInformationService.h"
#include <memory>

/**
 * @class User
 * @brief Concrete implementation of IUserContext for regular users
 * 
 * Represents a standard user in the movie ticket booking system with
 * basic privileges such as viewing movies, making bookings, and managing
 * personal account information.
 * 
 * @details
 * This class implements several design patterns:
 * - **Visitor Pattern**: Accepts service visitors for operations
 * - **Context Pattern**: Provides user-specific service context
 * - **Strategy Pattern**: Encapsulates user-specific behaviors
 * - **Shared Pointer Pattern**: Enables safe shared ownership
 * 
 * Key features:
 * - Account information management
 * - Booking history access
 * - Personal preferences
 * - Service delegation to appropriate handlers
 * 
 * @see IUserContext
 * @see Admin
 * @see Guest
 * @see AccountInformation
 * @since v1.0
 */
class User : public IUserContext, public std::enable_shared_from_this<User> {
private:
    /**
     * @brief Service for handling user information operations
     * 
     * Provides functionality for updating user details, preferences,
     * and account settings specific to regular users.
     */
    std::shared_ptr<UserInformationService> infoService;
    
public:
    /**
     * @brief Constructs a User with account information
     * 
     * Initializes a new User instance with the provided account details
     * and sets up the appropriate service handlers.
     * 
     * @param acc The account information for this user
     * 
     * @pre acc must contain valid user account data
     * @post User is ready to accept service operations
     * 
     * @throws std::invalid_argument if account information is invalid
     * 
     * @see AccountInformation
     * @since v1.0
     */
    User(const AccountInformation& acc);
    
    /**
     * @brief Accepts a service visitor for operations
     * 
     * Implements the Visitor pattern to allow various services to
     * operate on this user context without modifying the User class.
     * 
     * @param service The visitor service to accept
     * 
     * @pre service must be a valid visitor implementation
     * @post Visitor's visit method is called with this user context
     * 
     * @note This enables double dispatch for service operations
     * 
     * @see IVisitor
     * @see IServiceVisitor
     * @since v1.0
     */
    void accept(std::shared_ptr<IVisitor> service) override;
    
    /**
     * @brief Retrieves the user information service
     * 
     * Provides access to the service responsible for user-specific
     * information management operations.
     * 
     * @return std::shared_ptr<IUserInformationService> pointer to the service
     * 
     * @post Returns valid service instance
     * 
     * @note Service is lazily initialized if needed
     * 
     * @see IUserInformationService
     * @see UserInformationService
     * @since v1.0
     */
    std::shared_ptr<IUserInformationService> getUserInformationService() const override;
};

#endif
