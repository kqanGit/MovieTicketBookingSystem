/**
 * @file MovieMangerServiceVisitor.h
 * @brief Visitor implementation for role-based movie management service access control
 * @author MovieTicketBookingSystem Team
 * @date 2025
 * 
 * This file implements the Visitor pattern for providing role-specific access
 * to movie management services within the Movie Ticket Booking System.
 */

#ifndef MOVIEMANAGERSERVICEVISITOR_H
#define MOVIEMANAGERSERVICEVISITOR_H
#include "IServiceVisitor.h"
#include "../core/ServiceRegistry.h"
#include "../service/IMovieManagerService.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

/**
 * @class MovieManagerServiceVisitor
 * @brief Concrete visitor for managing role-based access to movie management services
 * 
 * The MovieManagerServiceVisitor implements the Visitor pattern to provide secure,
 * role-based access to movie management functionality. It ensures that movie
 * management operations are only accessible to users with appropriate privileges.
 * 
 * This class enforces the business rule that:
 * - Guests cannot access movie management services (read-only access only)
 * - Users cannot access movie management services (customers only)
 * - Admins can access full movie management services (CRUD operations)
 * 
 * @par Design Pattern
 * - **Visitor Pattern**: Separates movie management access logic from role objects
 * - **Service Locator**: Uses ServiceRegistry for dependency resolution
 * - **Authorization**: Enforces strict role-based access control
 * 
 * @par Security Considerations
 * This visitor implements a critical security boundary by ensuring that only
 * administrators can perform movie management operations like adding, updating,
 * or removing movies from the system catalog.
 * 
 * @par Business Rules
 * - Only administrators can create, update, or delete movies
 * - Movie management operations require elevated privileges
 * - All movie changes are logged for audit purposes
 * 
 * @par Thread Safety
 * This class is not thread-safe. External synchronization is required
 * for concurrent access in multi-threaded environments.
 * 
 * @see IServiceVisitor
 * @see IMovieManagerService
 * @see ServiceRegistry
 * 
 * @since 1.0
 */
class MovieManagerServiceVisitor : public IServiceVisitor {
private:
    /**
     * @brief Cached movie management service instance
     * 
     * Maintains a reference to the movie management service obtained from ServiceRegistry.
     * This caching improves performance by avoiding repeated service lookups.
     */
    std::shared_ptr<IMovieManagerService> _service;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes the visitor and retrieves the movie management service from ServiceRegistry.
     * 
     * @throws std::runtime_error If movie management service is not available in registry
     * 
     * @post _service contains valid movie management service reference
     */
    MovieManagerServiceVisitor();

    /**
     * @brief Retrieves the cached movie management service instance
     * 
     * Provides access to the movie management service for administrative operations.
     * This method ensures that only authorized roles interact with the service instance.
     * 
     * @return std::shared_ptr<IMovieManagerService> The movie management service instance
     * 
     * @pre MovieManagerServiceVisitor must be properly constructed
     * @post Returns valid movie management service reference
     * 
     * @see IMovieManagerService
     */
    std::shared_ptr<IMovieManagerService> getMovieManagerService();

    /**
     * @brief Denies movie management service access for Guest users
     * 
     * Guests are not authorized to perform movie management operations.
     * This method enforces the security constraint that prevents unauthenticated
     * users from accessing administrative movie management functionality.
     * 
     * @param role The guest role requesting service access (denied)
     * 
     * @pre role must be a valid Guest instance
     * @post No service access is granted to the guest
     * 
     * @note This method may log the access attempt for security monitoring
     * 
     * @see Guest
     */
    void service(std::shared_ptr<Guest> role) override;

    /**
     * @brief Denies movie management service access for regular Users
     * 
     * Regular users are customers and are not authorized to perform movie
     * management operations. This enforces the separation between customer
     * and administrative functionality.
     * 
     * @param role The user role requesting service access (denied)
     * 
     * @pre role must be a valid User instance
     * @post No service access is granted to the user
     * 
     * @note This method may log the access attempt for security monitoring
     * 
     * @see User
     */
    void service(std::shared_ptr<User> role) override;

    /**
     * @brief Provides movie management service access for Admin users
     * 
     * Enables administrators to access full movie management services including
     * creating, updating, deleting, and managing movie catalog operations.
     * 
     * @param role The admin role requesting service access
     * 
     * @pre role must be a valid Admin instance
     * @pre admin must have proper authorization
     * @post Admin has access to complete movie management service functionality
     * 
     * @par Available Operations
     * - Add new movies to the catalog
     * - Update existing movie information
     * - Remove movies from the catalog
     * - Manage movie showtimes and schedules
     * - Configure pricing and availability
     * 
     * @par Usage Example
     * @code
     * auto visitor = std::make_shared<MovieManagerServiceVisitor>();
     * auto admin = std::make_shared<Admin>();
     * visitor->service(admin);
     * auto movieService = visitor->getMovieManagerService();
     * movieService->addMovie(movieData);
     * @endcode
     * 
     * @see Admin
     * @see IMovieManagerService::addMovie
     * @see IMovieManagerService::updateMovie
     * @see IMovieManagerService::deleteMovie
     */
    void service(std::shared_ptr<Admin> role) override;
};

#endif