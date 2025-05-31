/**
 * @file BookingServiceVisitor.h
 * @brief Visitor implementation for booking service access control
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef _BOOKINGSERVICEVISITOR_H_
#define _BOOKINGSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "../core/ServiceRegistry.h"
#include "../service/IBookingService.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

/**
 * @class BookingServiceVisitor
 * @brief Visitor implementation for role-based booking service access
 * 
 * The BookingServiceVisitor class implements the Visitor pattern to provide
 * role-based access control for booking services in the movie ticket booking
 * system. This visitor manages access to booking functionality based on user
 * roles, ensuring that only authorized users can perform booking operations.
 * 
 * @details Key Responsibilities:
 * - Implement role-based access control for booking services
 * - Provide or restrict booking service access based on user roles
 * - Maintain booking service instance through ServiceRegistry
 * - Ensure security and authorization compliance
 * 
 * Design Pattern: Visitor Pattern
 * - Implements IServiceVisitor interface
 * - Provides double dispatch for role-specific service access
 * - Enables extensible role-based authorization
 * - Separates access control logic from service implementation
 * 
 * Access Control Rules:
 * - Guest: No access to booking services (must register/login first)
 * - User: Full access to booking services for personal bookings
 * - Admin: Full access to booking services including administrative operations
 * 
 * Security Features:
 * - Role verification before service access
 * - Centralized authorization logic
 * - Fail-safe access control (deny by default)
 * - Audit trail support through visitor pattern
 * 
 * Usage Example:
 * @code
 * auto bookingVisitor = std::make_unique<BookingServiceVisitor>();
 * 
 * // User requests booking service access
 * userRole->accept(bookingVisitor.get());
 * 
 * // Get service if authorized
 * auto bookingService = bookingVisitor->getBookingService();
 * if (bookingService) {
 *     // User is authorized for booking operations
 *     bookingService->createBooking(userId, showTimeId);
 * }
 * @endcode
 * 
 * @see IServiceVisitor
 * @see IBookingService
 * @see ServiceRegistry
 * @see Guest, User, Admin
 * @since 1.0
 */
class BookingServiceVisitor : public IServiceVisitor {
private:
    /**
     * @brief Booking service instance managed by this visitor
     * 
     * Shared pointer to the booking service obtained from ServiceRegistry.
     * This service is made available to authorized roles through the
     * visitor pattern implementation.
     */
    std::shared_ptr<IBookingService> _service;
    
public:
    /**
     * @brief Default constructor for BookingServiceVisitor
     * 
     * Creates a new BookingServiceVisitor instance and initializes the
     * booking service reference from the ServiceRegistry. The visitor
     * is ready to process role-based access requests after construction.
     * 
     * @post Visitor is initialized with booking service from registry
     * @post Service is ready to handle role-based access control
     * 
     * @throw std::runtime_error if ServiceRegistry is not properly initialized
     * @throw ServiceNotFoundException if booking service is not registered
     * 
     * @note Service is obtained from ServiceRegistry during construction
     * @note Visitor maintains service reference throughout its lifetime
     */
    BookingServiceVisitor();
    
    /**
     * @brief Retrieves the booking service for authorized users
     * 
     * Returns the booking service instance if the current user role has
     * been authorized through a previous visit operation. This method
     * provides access to booking functionality for qualified users.
     * 
     * @return Shared pointer to booking service if authorized, nullptr otherwise
     * 
     * @post Returns valid service pointer for authorized roles
     * @post Returns nullptr for unauthorized access attempts
     * 
     * @note Service access is determined by previous role visits
     * @note Guests will receive nullptr (no booking access)
     * @note Users and Admins receive valid service pointer
     * 
     * Usage:
     * @code
     * // After role has visited this visitor
     * auto service = bookingVisitor->getBookingService();
     * 
     * if (service) {
     *     // User is authorized for booking operations
     *     auto bookings = service->viewAllBookings(userId);
     *     service->createBooking(userId, showTimeId);
     * } else {
     *     // Access denied - user must login/register
     *     showLoginPrompt();
     * }
     * @endcode
     */
    std::shared_ptr<IBookingService> getBookingService();
    
    /**
     * @brief Processes booking service access for Guest role
     * 
     * Handles booking service access request from Guest users. Guests are
     * not authorized to access booking services as they must register or
     * login to make reservations. This method enforces the security policy
     * for anonymous users.
     * 
     * @param role Shared pointer to Guest role requesting service access
     * 
     * @pre role must not be null
     * @post Service access is denied for guest role
     * @post Internal service pointer remains null for guests
     * 
     * @note Guests cannot access booking services due to security policy
     * @note This enforces user registration/authentication requirements
     * @note Subsequent getBookingService() calls will return nullptr
     * 
     * Security Policy:
     * - Booking requires user authentication
     * - Anonymous users cannot make reservations
     * - Guest access is logged for security monitoring
     * 
     * Usage:
     * @code
     * auto guest = std::make_shared<Guest>();
     * bookingVisitor->service(guest);
     * 
     * // Service will be null - guests cannot book
     * auto service = bookingVisitor->getBookingService();
     * assert(service == nullptr);
     * @endcode
     */
    void service(std::shared_ptr<Guest> role) override;
    
    /**
     * @brief Processes booking service access for User role
     * 
     * Handles booking service access request from registered User accounts.
     * Users are authorized to access booking services for personal ticket
     * reservations and booking management. This method grants full booking
     * access to authenticated users.
     * 
     * @param role Shared pointer to User role requesting service access
     * 
     * @pre role must not be null
     * @pre User must be properly authenticated
     * @post Service access is granted for user role
     * @post Internal service pointer is set to valid booking service
     * 
     * @note Users get full access to personal booking operations
     * @note This includes creating, viewing, and managing personal bookings
     * @note Subsequent getBookingService() calls will return valid service
     * 
     * Authorized Operations:
     * - Create new bookings for showtimes
     * - View personal booking history
     * - Cancel personal bookings (if policy allows)
     * - Select seats for reservations
     * 
     * Usage:
     * @code
     * auto user = std::make_shared<User>(accountInfo);
     * bookingVisitor->service(user);
     * 
     * // Service is now available for user operations
     * auto service = bookingVisitor->getBookingService();
     * service->createBooking(user->getId(), showTimeId);
     * @endcode
     */
    void service(std::shared_ptr<User> role) override;
    
    /**
     * @brief Processes booking service access for Admin role
     * 
     * Handles booking service access request from Administrator accounts.
     * Admins are authorized to access all booking services including both
     * personal operations and administrative functions. This method grants
     * comprehensive booking access to administrative users.
     * 
     * @param role Shared pointer to Admin role requesting service access
     * 
     * @pre role must not be null
     * @pre Admin must be properly authenticated
     * @post Service access is granted for admin role
     * @post Internal service pointer is set to valid booking service
     * 
     * @note Admins get full access to all booking operations
     * @note This includes both personal and administrative booking functions
     * @note Subsequent getBookingService() calls will return valid service
     * 
     * Authorized Operations:
     * - All standard user booking operations
     * - View all users' booking history (administrative)
     * - Cancel any bookings (administrative)
     * - Generate booking reports and statistics
     * - Handle booking disputes and modifications
     * 
     * Usage:
     * @code
     * auto admin = std::make_shared<Admin>(accountInfo);
     * bookingVisitor->service(admin);
     * 
     * // Service is now available for admin operations
     * auto service = bookingVisitor->getBookingService();
     * 
     * // Admin can perform both personal and administrative operations
     * service->createBooking(admin->getId(), showTimeId);
     * auto allBookings = service->viewAllSystemBookings(); // Admin function
     * @endcode
     */
    void service(std::shared_ptr<Admin> role) override;
};
#endif