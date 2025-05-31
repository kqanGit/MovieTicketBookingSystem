/**
 * @file MovieManagerService.h
 * @brief Concrete implementation of movie management service
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

// MovieManagerService.h
#ifndef MOVIEMANAGERSERVICE_H
#define MOVIEMANAGERSERVICE_H

#include "IMovieManagerService.h"
#include "../repository/IMovieRepository.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/**
 * @class MovieManagerService
 * @brief Concrete implementation of movie management operations
 * 
 * The MovieManagerService class provides the concrete implementation of all
 * movie management functionality for cinema administrators. This service
 * handles movie catalog management, showtime scheduling, and administrative
 * operations while ensuring data integrity and business rule compliance.
 * 
 * @details Implementation Features:
 * - Integrates with IMovieRepository for data persistence
 * - Validates business rules for movie management
 * - Handles transaction management for complex operations
 * - Provides error handling and logging for administrative actions
 * 
 * Design Pattern: Service Pattern
 * - Implements IMovieManagerService interface
 * - Uses Repository pattern for data access
 * - Encapsulates movie management business logic
 * - Provides dependency injection through constructor
 * 
 * Business Rules Enforced:
 * - Movie titles must be unique within the system
 * - Showtimes must be in valid format and future dates
 * - Movies with active bookings cannot be deleted
 * - Showtime conflicts are prevented through validation
 * 
 * Usage Example:
 * @code
 * auto movieRepo = std::make_shared<MovieRepositorySQL>(dbConnection);
 * auto movieManager = std::make_unique<MovieManagerService>(movieRepo);
 * 
 * // Add a new movie with showtimes
 * auto movie = std::make_shared<Movie>("Inception", 148, "Sci-Fi");
 * std::vector<std::string> times = {"14:00", "17:30", "21:00"};
 * movieManager->addMovie(movie, times);
 * @endcode
 * 
 * @see IMovieManagerService
 * @see IMovieRepository
 * @see IMovie
 * @since 1.0
 */
class MovieManagerService : public IMovieManagerService {
private:
    /**
     * @brief Repository for movie data access operations
     * 
     * Shared pointer to the movie repository implementation used for
     * all database operations. This follows the dependency injection
     * pattern, allowing for different repository implementations.
     */
    std::shared_ptr<IMovieRepository> repo;

public:
    /**
     * @brief Constructs MovieManagerService with repository dependency
     * 
     * Creates a new MovieManagerService instance with the provided movie
     * repository. The repository is used for all data persistence operations
     * and must remain valid for the lifetime of the service.
     * 
     * @param r Shared pointer to movie repository implementation
     * 
     * @pre r must not be null
     * @pre Repository must be properly initialized and connected
     * @post Service is ready for movie management operations
     * 
     * @throw std::invalid_argument if repository pointer is null
     * 
     * @note Repository is stored as shared_ptr to allow shared ownership
     * @note Service lifetime is independent of repository creation context
     * 
     * Usage:
     * @code
     * auto movieRepo = std::make_shared<MovieRepositorySQL>(connection);
     * auto movieManager = std::make_unique<MovieManagerService>(movieRepo);
     * 
     * // Service is ready to use
     * movieManager->addMovie(movie, showtimes);
     * @endcode
     */
    explicit MovieManagerService(std::shared_ptr<IMovieRepository> r);
    
    /**
     * @brief Adds a new movie with showtimes to the system catalog
     * 
     * Creates a new movie entry in the system database along with the
     * specified showtimes. This method validates all input data, checks
     * for conflicts, and ensures transactional consistency.
     * 
     * @param movie Shared pointer to the movie object to add
     * @param ShowTimes Vector of showtime strings in HH:MM format
     * 
     * @pre movie must not be null and contain valid data
     * @pre ShowTimes must not be empty and contain valid time formats
     * @pre Movie title must not already exist in the system
     * @pre Showtimes must not conflict with existing schedules
     * @post Movie is added to the system catalog
     * @post All showtimes are created and associated with the movie
     * @post Movie becomes available for booking
     * 
     * @throw std::invalid_argument if movie is null or data is invalid
     * @throw DuplicateMovieException if movie title already exists
     * @throw InvalidShowTimeException if showtime format is invalid
     * @throw ScheduleConflictException if showtimes conflict
     * @throw std::runtime_error if database operation fails
     * 
     * @note Operation is transactional - either all data is saved or none
     * @note Showtime validation includes format and future date checks
     * @note Movie title uniqueness is enforced at the service level
     * 
     * Implementation Details:
     * - Validates movie object completeness
     * - Parses and validates showtime formats
     * - Checks for title uniqueness before insertion
     * - Uses repository transaction support if available
     * - Logs administrative actions for audit purposes
     * 
     * Usage:
     * @code
     * auto movie = std::make_shared<Movie>("The Matrix", 136, "Action");
     * std::vector<std::string> showtimes = {"10:00", "14:30", "18:00", "21:30"};
     * 
     * try {
     *     movieManager->addMovie(movie, showtimes);
     *     std::cout << "Movie added successfully!" << std::endl;
     * } catch (const DuplicateMovieException& e) {
     *     std::cout << "Movie already exists: " << e.what() << std::endl;
     * } catch (const std::exception& e) {
     *     std::cout << "Failed to add movie: " << e.what() << std::endl;
     * }
     * @endcode
     */
    void addMovie(std::shared_ptr<IMovie> movie, std::vector<std::string> ShowTimes) override;
    
    /**
     * @brief Removes a movie and all associated data from the system
     * 
     * Completely removes a movie from the system catalog, including all
     * showtimes, seat configurations, and related data. This operation
     * includes validation to prevent deletion of movies with active bookings.
     * 
     * @param id Unique identifier of the movie to delete
     * 
     * @pre id must be a valid, existing movie identifier
     * @pre Movie should not have active future bookings
     * @post Movie is removed from the system catalog
     * @post All associated showtimes are deleted
     * @post All related configurations are cleaned up
     * @post Movie is no longer available for booking
     * 
     * @throw std::invalid_argument if id is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw ActiveBookingsException if movie has active bookings
     * @throw std::runtime_error if deletion operation fails
     * 
     * @note Considers implementing soft deletion for audit purposes
     * @note Validates no active bookings exist before deletion
     * @note Operation is transactional for data consistency
     * 
     * Implementation Details:
     * - Verifies movie existence before deletion
     * - Checks for active bookings that would prevent deletion
     * - Cascades deletion to all related entities
     * - Uses database transactions for consistency
     * - Logs deletion actions for audit trail
     * 
     * Usage:
     * @code
     * try {
     *     movieManager->deleteMovie(movieId);
     *     std::cout << "Movie deleted successfully!" << std::endl;
     * } catch (const ActiveBookingsException& e) {
     *     std::cout << "Cannot delete movie with active bookings." << std::endl;
     * } catch (const MovieNotFoundException& e) {
     *     std::cout << "Movie not found." << std::endl;
     * }
     * @endcode
     */
    void deleteMovie(int id) override;
    
    /**
     * @brief Removes a specific showtime from a movie
     * 
     * Deletes a single showtime entry without affecting the movie or other
     * showtimes. This method validates the relationship between movie and
     * showtime and checks for active bookings before deletion.
     * 
     * @param movieId Unique identifier of the movie
     * @param ShowTimeId Unique identifier of the showtime to delete
     * 
     * @pre movieId must be a valid, existing movie identifier
     * @pre ShowTimeId must be a valid, existing showtime identifier
     * @pre ShowTimeId must belong to the specified movie
     * @pre Showtime should not have active bookings
     * @post Specified showtime is removed from the movie
     * @post Other showtimes remain unaffected
     * @post Showtime is no longer available for booking
     * 
     * @throw std::invalid_argument if movieId or ShowTimeId is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw ShowTimeNotFoundException if showtime does not exist
     * @throw ShowTimeMismatchException if showtime doesn't belong to movie
     * @throw ActiveBookingsException if showtime has active bookings
     * @throw std::runtime_error if deletion fails
     * 
     * @note Validates showtime ownership by the specified movie
     * @note Checks for customer bookings before deletion
     * @note May trigger customer notifications for affected bookings
     * 
     * Implementation Details:
     * - Verifies movie and showtime existence
     * - Validates showtime belongs to the specified movie
     * - Checks for any customer bookings for the showtime
     * - Handles customer notification if bookings exist
     * - Uses transactions for data consistency
     * 
     * Usage:
     * @code
     * try {
     *     movieManager->deleteShowTime(movieId, showTimeId);
     *     std::cout << "Showtime removed successfully!" << std::endl;
     * } catch (const ActiveBookingsException& e) {
     *     std::cout << "Cannot remove showtime with active bookings." << std::endl;
     * } catch (const ShowTimeMismatchException& e) {
     *     std::cout << "Showtime does not belong to this movie." << std::endl;
     * }
     * @endcode
     */
    void deleteShowTime(int movieId, int ShowTimeId) override;
};

#endif // MOVIEMANAGERSERVICE_H