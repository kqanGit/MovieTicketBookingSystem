/**
 * @file IMovieManagerService.h
 * @brief Movie management service interface definition
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef IMOVIEMANAGERSERVICE_H
#define IMOVIEMANAGERSERVICE_H

#include "../model/IMovie.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>

/**
 * @interface IMovieManagerService
 * @brief Interface for movie management operations
 * 
 * The IMovieManagerService interface defines the contract for all movie
 * management operations in the cinema booking system. This interface
 * provides administrative functionality for managing movies and showtimes,
 * typically used by cinema administrators and managers.
 * 
 * @details Key Responsibilities:
 * - Add new movies to the system with showtimes
 * - Remove movies from the catalog
 * - Manage individual showtimes for movies
 * - Maintain movie catalog integrity
 * 
 * Design Pattern: Service Pattern
 * - Encapsulates movie management business logic
 * - Provides abstraction for different management implementations
 * - Enables dependency injection and unit testing
 * - Separates administrative concerns from data access
 * 
 * Security Considerations:
 * - Operations should verify administrative privileges
 * - Input validation should be performed at service layer
 * - Deletion operations should check for existing bookings
 * - Audit logging should be implemented for all operations
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<IMovieManagerService> movieManager = 
 *     std::make_unique<MovieManagerService>(movieRepo);
 * 
 * // Add a new movie with showtimes
 * auto movie = std::make_shared<Movie>("Inception", 148, "Sci-Fi");
 * std::vector<std::string> times = {"14:00", "17:30", "21:00"};
 * movieManager->addMovie(movie, times);
 * 
 * // Remove a specific showtime
 * movieManager->deleteShowTime(movieId, showTimeId);
 * @endcode
 * 
 * @see MovieManagerService
 * @see IMovie
 * @see IMovieRepository
 * @since 1.0
 */
class IMovieManagerService {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     * 
     * Ensures that concrete movie manager service implementations are properly
     * destroyed when accessed through the interface pointer.
     */
    virtual ~IMovieManagerService() {}
    
    /**
     * @brief Adds a new movie to the system with specified showtimes
     * 
     * Creates a new movie entry in the system catalog along with the provided
     * showtimes. This method handles both movie registration and initial
     * showtime scheduling in a single operation.
     * 
     * @param movie Shared pointer to the movie object to add
     * @param ShowTimes Vector of showtime strings (e.g., "14:30", "18:00")
     * 
     * @pre movie must not be null
     * @pre movie must have valid title, duration, and genre
     * @pre ShowTimes must not be empty
     * @pre All showtime strings must be in valid time format
     * @pre Showtimes should not conflict with existing schedules
     * @post Movie is added to the system catalog
     * @post All specified showtimes are created and associated with the movie
     * @post Movie becomes available for booking
     * 
     * @throw std::invalid_argument if movie is null or showtimes are invalid
     * @throw DuplicateMovieException if movie already exists
     * @throw ScheduleConflictException if showtimes conflict with existing ones
     * @throw std::runtime_error if database operation fails
     * 
     * @note Operation should be atomic - either all data is saved or none
     * @note Showtime format should follow HH:MM pattern
     * @note Consider timezone handling for showtime scheduling
     * 
     * Usage:
     * @code
     * auto movie = std::make_shared<Movie>("Avatar 2", 192, "Action");
     * std::vector<std::string> showtimes = {"10:00", "14:30", "18:00", "21:30"};
     * 
     * try {
     *     movieManager->addMovie(movie, showtimes);
     *     // Movie and showtimes successfully added
     * } catch (const std::exception& e) {
     *     // Handle addition failure
     * }
     * @endcode
     */
    virtual void addMovie(std::shared_ptr<IMovie> movie, std::vector<std::string> ShowTimes) = 0;
    
    /**
     * @brief Removes a movie and all associated data from the system
     * 
     * Completely removes a movie from the system catalog, including all
     * associated showtimes, seat configurations, and related data. This
     * operation should handle cascading deletions appropriately.
     * 
     * @param id Unique identifier of the movie to delete
     * 
     * @pre id must be a valid, existing movie identifier
     * @pre Movie should not have active bookings (implementation dependent)
     * @post Movie is removed from the system catalog
     * @post All associated showtimes are deleted
     * @post All related seat configurations are removed
     * @post Movie is no longer available for booking
     * 
     * @throw std::invalid_argument if id is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw ActiveBookingsException if movie has active bookings
     * @throw std::runtime_error if deletion operation fails
     * 
     * @note Consider soft deletion for audit purposes
     * @note Check for existing bookings before deletion
     * @note Operation should be transactional
     * 
     * Usage:
     * @code
     * try {
     *     movieManager->deleteMovie(movieId);
     *     // Movie successfully removed
     * } catch (const ActiveBookingsException& e) {
     *     // Handle case where movie has active bookings
     * } catch (const std::exception& e) {
     *     // Handle other deletion failures
     * }
     * @endcode
     */
    virtual void deleteMovie(int id) = 0;
    
    /**
     * @brief Removes a specific showtime from a movie
     * 
     * Deletes a single showtime entry from the specified movie without
     * affecting the movie itself or other showtimes. This allows for
     * granular schedule management.
     * 
     * @param movieId Unique identifier of the movie
     * @param ShowTimeId Unique identifier of the showtime to delete
     * 
     * @pre movieId must be a valid, existing movie identifier
     * @pre ShowTimeId must be a valid, existing showtime identifier
     * @pre ShowTimeId must belong to the specified movie
     * @pre Showtime should not have active bookings (implementation dependent)
     * @post Specified showtime is removed from the movie
     * @post Other showtimes for the movie remain unaffected
     * @post Showtime is no longer available for booking
     * 
     * @throw std::invalid_argument if movieId or ShowTimeId is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw ShowTimeNotFoundException if showtime does not exist
     * @throw ShowTimeMismatchException if showtime doesn't belong to movie
     * @throw ActiveBookingsException if showtime has active bookings
     * @throw std::runtime_error if deletion operation fails
     * 
     * @note Verify showtime belongs to the specified movie
     * @note Check for existing bookings for the showtime
     * @note Consider notification to affected customers
     * 
     * Usage:
     * @code
     * try {
     *     movieManager->deleteShowTime(movieId, showTimeId);
     *     // Showtime successfully removed
     * } catch (const ActiveBookingsException& e) {
     *     // Handle case where showtime has bookings
     * } catch (const std::exception& e) {
     *     // Handle other deletion failures
     * }
     * @endcode
     */
    virtual void deleteShowTime(int movieId, int ShowTimeId) = 0;
};

#endif // IMOVIEMANAGERSERVICE_H