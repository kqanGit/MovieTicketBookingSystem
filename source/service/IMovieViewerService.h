/**
 * @file IMovieViewerService.h
 * @brief Movie viewing service interface definition
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef IMOVIEVIEWERSERVICE_H
#define IMOVIEVIEWERSERVICE_H

#include <vector>
#include "../repository/MovieDTO.h"
#include "../model/Movie.h"
#include "../model/ShowTime.h"
#include <memory>

/**
 * @interface IMovieViewerService
 * @brief Interface for movie browsing and viewing operations
 * 
 * The IMovieViewerService interface defines the contract for all movie
 * viewing and browsing operations in the cinema booking system. This
 * interface provides read-only access to movie information and is used
 * by both guests and registered users to explore available movies.
 * 
 * @details Key Responsibilities:
 * - Display available movies in the catalog
 * - Provide detailed information about specific movies
 * - Show available showtimes for movies
 * - Support movie browsing and discovery
 * 
 * Design Pattern: Service Pattern
 * - Encapsulates movie viewing business logic
 * - Provides abstraction for different viewing implementations
 * - Enables caching and performance optimizations
 * - Separates presentation logic from data access
 * 
 * Performance Considerations:
 * - Methods may implement caching for frequently accessed data
 * - Large movie catalogs should support pagination
 * - Showtime queries should be optimized for date ranges
 * - Consider lazy loading for detailed movie information
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<IMovieViewerService> movieViewer = 
 *     std::make_unique<MovieViewerService>(movieRepo);
 * 
 * // Browse all available movies
 * auto movies = movieViewer->showAllMovies();
 * 
 * // Get details for a specific movie
 * auto movieDetail = movieViewer->showMovieDetail(movieId);
 * 
 * // View showtimes for the movie
 * auto showtimes = movieViewer->showMovieShowTimes(movieId);
 * @endcode
 * 
 * @see MovieViewerService
 * @see MovieDTO
 * @see IMovie
 * @see ShowTime
 * @since 1.0
 */
class IMovieViewerService {
public:    
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     * 
     * Ensures that concrete movie viewer service implementations are properly
     * destroyed when accessed through the interface pointer.
     */
    virtual ~IMovieViewerService() {}
    
    /**
     * @brief Retrieves all available movies in the system catalog
     * 
     * Returns a comprehensive list of all movies currently available in the
     * cinema system. This method provides a summary view of movies suitable
     * for catalog browsing and selection interfaces.
     * 
     * @return Vector of MovieDTO objects containing movie summary information
     * 
     * @post Returns all active movies in the system
     * @post MovieDTO objects contain essential movie information
     * @post Results may be cached for performance
     * 
     * @throw std::runtime_error if database query fails
     * @throw ServiceUnavailableException if movie service is temporarily unavailable
     * 
     * @note Returns only active/available movies, not removed ones
     * @note Results may be ordered by various criteria (title, release date, etc.)
     * @note Empty vector is returned if no movies are available
     * @note Consider implementing pagination for large catalogs
     * 
     * Usage:
     * @code
     * try {
     *     auto allMovies = movieViewer->showAllMovies();
     *     for (const auto& movie : allMovies) {
     *         std::cout << movie.getTitle() << " - " 
     *                   << movie.getDuration() << " min" << std::endl;
     *     }
     * } catch (const std::exception& e) {
     *     // Handle movie retrieval failure
     * }
     * @endcode
     */
    virtual std::vector<MovieDTO> showAllMovies() = 0;
    
    /**
     * @brief Retrieves detailed information for a specific movie
     * 
     * Returns comprehensive details about a specific movie, including all
     * movie properties and metadata. This method provides the full movie
     * entity for detailed viewing and booking operations.
     * 
     * @param id Unique identifier of the movie to retrieve
     * @return Shared pointer to the movie entity with complete details
     * 
     * @pre id must be a valid, existing movie identifier
     * @post Returns complete movie information
     * @post Movie object contains all available properties
     * 
     * @throw std::invalid_argument if id is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw std::runtime_error if database query fails
     * 
     * @note Returns null pointer if movie is not found (implementation dependent)
     * @note Movie details may include extended information not in DTO
     * @note Consider caching frequently accessed movie details
     * 
     * Usage:
     * @code
     * try {
     *     auto movieDetail = movieViewer->showMovieDetail(selectedMovieId);
     *     if (movieDetail) {
     *         std::cout << "Title: " << movieDetail->getTitle() << std::endl;
     *         std::cout << "Genre: " << movieDetail->getGenre() << std::endl;
     *         std::cout << "Duration: " << movieDetail->getDuration() << " minutes" << std::endl;
     *     }
     * } catch (const MovieNotFoundException& e) {
     *     // Handle movie not found
     * }
     * @endcode
     */
    virtual std::shared_ptr<IMovie> showMovieDetail(int id) = 0;
    
    /**
     * @brief Retrieves all available showtimes for a specific movie
     * 
     * Returns a list of all scheduled showtimes for the specified movie,
     * enabling users to select their preferred screening time for booking.
     * Showtimes may be filtered to show only future/available screenings.
     * 
     * @param id Unique identifier of the movie to query showtimes for
     * @return Vector of ShowTime objects containing scheduling information
     * 
     * @pre id must be a valid, existing movie identifier
     * @post Returns all available showtimes for the movie
     * @post ShowTime objects contain complete scheduling information
     * @post Results may be filtered to show only future showtimes
     * 
     * @throw std::invalid_argument if id is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw std::runtime_error if database query fails
     * 
     * @note May return only future showtimes or all showtimes (implementation dependent)
     * @note Empty vector is returned if no showtimes are scheduled
     * @note Results may be ordered chronologically
     * @note Consider filtering by date range for better performance
     * 
     * Usage:
     * @code
     * try {
     *     auto showtimes = movieViewer->showMovieShowTimes(movieId);
     *     for (const auto& showtime : showtimes) {
     *         std::cout << "Showtime: " << showtime.getDateTime() 
     *                   << " - Theater: " << showtime.getTheater() << std::endl;
     *     }
     * } catch (const MovieNotFoundException& e) {
     *     // Handle movie not found
     * }
     * @endcode
     */
    virtual std::vector<ShowTime> showMovieShowTimes(int id) = 0;
};

#endif // IMOVIEVIEWERSERVICE_H