/**
 * @file Guest.h
 * @brief Guest user context implementation for anonymous users
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef GUEST_H
#define GUEST_H
#include "../visitor/IVisitor.h"
#include "../context/IUserContext.h"
#include "../visitor/IServiceVisitor.h"
#include <memory> // Thêm để sử dụng unique_ptr

/**
 * @class Guest
 * @brief Implementation of IUserContext for anonymous guest users
 * 
 * This class represents an anonymous guest user who can browse the movie
 * booking system without authentication. Guests have limited access to
 * system features and cannot perform authenticated operations like booking tickets.
 * 
 * @details
 * Key Features:
 * - Anonymous browsing capabilities
 * - Limited system access without authentication
 * - Visitor pattern support for role-based operations
 * - Read-only access to public information
 * 
 * Design Patterns:
 * - **Strategy Pattern**: Implements IUserContext with guest-specific behavior
 * - **Visitor Pattern**: Accepts visitors for role-based service operations
 * - **Null Object Pattern**: Provides default behavior for unauthenticated users
 * - **Factory Pattern**: Created through GuestContextCreator
 * 
 * Guest Capabilities:
 * - Browse available movies and showtimes
 * - View movie details and information
 * - Access public content and features
 * - Navigate the system interface
 * 
 * Guest Limitations:
 * - Cannot book tickets or make reservations
 * - No access to user-specific features
 * - Cannot save preferences or history
 * - No payment or transaction capabilities
 * 
 * @par Usage Example
 * @code
 * auto guest = std::make_shared<Guest>();
 * 
 * // Guest can browse movies
 * auto serviceVisitor = std::make_shared<MovieBrowsingVisitor>();
 * guest->accept(serviceVisitor);
 * 
 * // But cannot access user-specific services
 * auto userInfo = guest->getUserInformationService(); // Returns limited service
 * @endcode
 * 
 * @note Guests should be encouraged to register for full system access
 * @warning Do not allow guests to access sensitive or user-specific data
 * 
 * @see IUserContext
 * @see GuestContextCreator
 * @see IVisitor
 * @see IServiceVisitor
 * @see User
 */
class Guest : public IUserContext, public std::enable_shared_from_this<Guest> {
public:
    /**
     * @brief Default constructor
     * 
     * Creates a new guest context with default anonymous user settings.
     * Initializes the guest for basic system browsing capabilities.
     * 
     * @post Guest context is ready for visitor pattern operations
     * @post Guest has limited system access permissions
     * 
     * @note No authentication or user data is associated with guests
     */
    Guest() = default;
    
    /**
     * @brief Accept a visitor for service operations
     * 
     * Implements the visitor pattern to allow different service visitors
     * to operate on the guest context. This enables role-based service
     * access with guest-specific limitations.
     * 
     * @param service Visitor implementing service operations for guests
     * 
     * @pre service is not null
     * @post Visitor has performed guest-appropriate operations
     * 
     * @par Visitor Behavior
     * - Service visitors should respect guest limitations
     * - Only provide read-only and public operations
     * - Block authenticated-only features
     * - Guide guests toward registration when appropriate
     * 
     * @note Visitors must check user context type for proper access control
     * @see IVisitor
     * @see IServiceVisitor
     * 
     * @par Example
     * @code
     * auto guest = std::make_shared<Guest>();
     * auto movieBrowser = std::make_shared<MovieBrowsingVisitor>();
     * guest->accept(movieBrowser); // Allows movie browsing
     * 
     * auto bookingService = std::make_shared<BookingServiceVisitor>();
     * guest->accept(bookingService); // Should provide limited/redirect behavior
     * @endcode
     */
    void accept(std::shared_ptr<IVisitor> service) override;
    
    /**
     * @brief Get user information service for guests
     * 
     * Returns a user information service appropriate for guest contexts.
     * This service provides limited functionality compared to authenticated
     * user services.
     * 
     * @return std::shared_ptr<IUserInformationService> Guest-appropriate information service
     * 
     * @post Returns service with guest-level access
     * @post Service provides limited functionality
     * 
     * @par Guest Service Features
     * - Basic system information access
     * - Public content browsing
     * - No personal data access
     * - Registration encouragement prompts
     * 
     * @note Service should encourage guest registration for full access
     * @warning Service must not expose user-specific or sensitive data
     * 
     * @see IUserInformationService
     * @see IUserContext::getUserInformationService()
     * 
     * @par Example
     * @code
     * auto guest = std::make_shared<Guest>();
     * auto infoService = guest->getUserInformationService();
     * 
     * // Can access public information
     * auto publicInfo = infoService->getPublicSystemInfo();
     * 
     * // Cannot access user-specific data
     * // auto userData = infoService->getUserData(); // Should return null/empty
     * @endcode
     */
    std::shared_ptr<IUserInformationService> getUserInformationService() const override;
    
    /**
     * @brief Virtual destructor
     * 
     * Ensures proper cleanup of guest context resources.
     * Since guests have minimal state, cleanup is typically minimal.
     * 
     * @note Automatically called when guest context is destroyed
     */
    virtual ~Guest() = default;
};

#endif