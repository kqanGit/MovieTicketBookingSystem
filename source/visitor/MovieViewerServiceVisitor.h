/**
 * @file MovieViewerServiceVisitor.h
 * @brief Visitor implementation for role-based movie viewing service access control
 * @author MovieTicketBookingSystem Team
 * @date 2025
 * 
 * This file implements the Visitor pattern for providing role-specific access
 * to movie viewing services within the Movie Ticket Booking System.
 */

#ifndef MOVIEVIEWERSERVICEVISITOR_H
#define MOVIEVIEWERSERVICEVISITOR_H
#include "IServiceVisitor.h"
#include "../service/IMovieViewerService.h"
#include "../core/ServiceRegistry.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

/**
 * @class MovieViewerServiceVisitor
 * @brief Concrete visitor for managing role-based access to movie viewing services
 * 
 * The MovieViewerServiceVisitor implements the Visitor pattern to provide universal
 * access to movie viewing functionality. Unlike management services, movie viewing
 * is generally accessible to all user roles with appropriate presentation based on
 * their authentication status.
 * 
 * This class implements the business rule that:
 * - Guests can access movie viewing services (browse movies, view details)
 * - Users can access movie viewing services (enhanced features, personalization)
 * - Admins can access movie viewing services (full catalog access, preview modes)
 * 
 * @par Design Pattern
 * - **Visitor Pattern**: Separates movie viewing access logic from role objects
 * - **Service Locator**: Uses ServiceRegistry for dependency resolution
 * - **Universal Access**: Provides appropriate viewing experience for each role
 * 
 * @par Access Control Strategy
 * Unlike restrictive services, movie viewing implements permissive access control
 * where all roles can view movies but with different feature sets and presentation
 * modes based on their authorization level.
 * 
 * @par Performance Considerations
 * This visitor supports high-frequency access patterns as movie browsing is
 * the primary use case for most system users. Caching strategies should be
 * implemented at the service level for optimal performance.
 * 
 * @par Thread Safety
 * This class is not thread-safe. External synchronization is required
 * for concurrent access in multi-threaded environments.
 * 
 * @see IServiceVisitor
 * @see IMovieViewerService
 * @see ServiceRegistry
 * 
 * @since 1.0
 */


class MovieViewerServiceVisitor : public IServiceVisitor {
private:
    /**
     * @brief Cached movie viewing service instance
     * 
     * Maintains a reference to the movie viewing service obtained from ServiceRegistry.
     * This caching improves performance for the high-frequency movie browsing operations.
     */
    std::shared_ptr<IMovieViewerService> _service;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes the visitor and retrieves the movie viewing service from ServiceRegistry.
     * 
     * @throws std::runtime_error If movie viewing service is not available in registry
     * 
     * @post _service contains valid movie viewing service reference
     */
    MovieViewerServiceVisitor();

    /**
     * @brief Retrieves the cached movie viewing service instance
     * 
     * Provides access to the movie viewing service for role-appropriate browsing operations.
     * This method ensures consistent service access across all user roles.
     * 
     * @return std::shared_ptr<IMovieViewerService> The movie viewing service instance
     * 
     * @pre MovieViewerServiceVisitor must be properly constructed
     * @post Returns valid movie viewing service reference
     * 
     * @see IMovieViewerService
     */
    std::shared_ptr<IMovieViewerService> getMovieViewerService();

    /**
     * @brief Provides movie viewing service access for Guest users
     * 
     * Enables guest users to browse the movie catalog and view movie details.
     * Guests receive basic movie information suitable for browsing and selection.
     * 
     * @param role The guest role requesting service access
     * 
     * @pre role must be a valid Guest instance
     * @post Guest has access to movie viewing service with basic features
     * 
     * @par Available Features for Guests
     * - Browse movie catalog
     * - View movie details (title, description, rating, duration)
     * - Check showtime availability
     * - View theater information
     * 
     * @par Usage Example
     * @code
     * auto visitor = std::make_shared<MovieViewerServiceVisitor>();
     * auto guest = std::make_shared<Guest>();
     * visitor->service(guest);
     * auto viewerService = visitor->getMovieViewerService();
     * auto movies = viewerService->getAvailableMovies();
     * @endcode
     * 
     * @see Guest
     * @see IMovieViewerService::getAvailableMovies
     */
    void service(std::shared_ptr<Guest> role) override;

    /**
     * @brief Provides movie viewing service access for authenticated Users
     * 
     * Allows authenticated users to access enhanced movie viewing features including
     * personalized recommendations and viewing history.
     * 
     * @param role The user role requesting service access
     * 
     * @pre role must be a valid User instance
     * @post User has access to movie viewing service with enhanced features
     * 
     * @par Enhanced Features for Users
     * - All guest features plus:
     * - Personalized movie recommendations
     * - Viewing history tracking
     * - Favorite movies list
     * - Advanced filtering and search options
     * - Price information for authenticated users
     * 
     * @see User
     * @see IMovieViewerService::getPersonalizedRecommendations
     */
    void service(std::shared_ptr<User> role) override;

    /**
     * @brief Provides movie viewing service access for Admin users
     * 
     * Enables administrators to access full movie viewing capabilities including
     * administrative features and preview modes for unreleased content.
     * 
     * @param role The admin role requesting service access
     * 
     * @pre role must be a valid Admin instance
     * @post Admin has access to complete movie viewing service functionality
     * 
     * @par Administrative Features
     * - All user features plus:
     * - Preview unreleased movies
     * - View detailed analytics and statistics
     * - Access to administrative movie metadata
     * - Preview mode for testing new content
     * - Full catalog access including hidden/disabled movies
     * 
     * @see Admin
     * @see IMovieViewerService::getAdminMovieDetails
     */
    void service(std::shared_ptr<Admin> role) override;
};


#endif 