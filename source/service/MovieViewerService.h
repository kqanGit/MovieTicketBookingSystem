/**
 * @file MovieViewerService.h
 * @brief Concrete implementation of movie viewing service
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

// MovieViewerService.h
#ifndef MOVIEVIEWERSERVICE_H
#define MOVIEVIEWERSERVICE_H

#include "IMovieViewerService.h"
#include "../repository/IMovieRepository.h"
#include <memory>
#include <vector>   
#include "../model/ShowTime.h"  // Include ShowTime model

/**
 * @class MovieViewerService
 * @brief Concrete implementation of movie browsing and viewing operations
 * 
 * The MovieViewerService class provides the concrete implementation for all
 * movie viewing and browsing functionality in the cinema booking system.
 * This service enables both guests and registered users to explore the
 * movie catalog, view details, and check available showtimes.
 * 
 * @details Implementation Features:
 * - Integrates with IMovieRepository for data access
 * - Provides caching for frequently accessed movie data
 * - Handles data transformation between repository and presentation layers
 * - Supports efficient querying for large movie catalogs
 * 
 * Design Pattern: Service Pattern
 * - Implements IMovieViewerService interface
 * - Uses Repository pattern for data access abstraction
 * - Encapsulates movie viewing business logic
 * - Provides dependency injection through constructor
 * 
 * Performance Features:
 * - Optimized queries for movie catalog browsing
 * - Efficient showtime retrieval with date filtering
 * - Caching support for frequently accessed movies
 * - Lazy loading for detailed movie information
 * 
 * Usage Example:
 * @code
 * auto movieRepo = std::make_shared<MovieRepositorySQL>(dbConnection);
 * auto movieViewer = std::make_unique<MovieViewerService>(movieRepo);
 * 
 * // Browse available movies
 * auto movies = movieViewer->showAllMovies();
 * 
 * // Get details for selection
 * auto detail = movieViewer->showMovieDetail(selectedId);
 * auto showtimes = movieViewer->showMovieShowTimes(selectedId);
 * @endcode
 * 
 * @see IMovieViewerService
 * @see IMovieRepository
 * @see MovieDTO
 * @see ShowTime
 * @since 1.0
 */
class MovieViewerService : public IMovieViewerService {
private:
    /**
     * @brief Repository for movie data access operations
     * 
     * Shared pointer to the movie repository implementation used for
     * all movie data retrieval operations. This follows the dependency
     * injection pattern for flexible repository implementations.
     */
    std::shared_ptr<IMovieRepository> repo;  // Using smart pointer

public:
    /**
     * @brief Constructs MovieViewerService with repository dependency
     * 
     * Creates a new MovieViewerService instance with the provided movie
     * repository for data access. The repository must be properly initialized
     * and remain valid throughout the service lifetime.
     * 
     * @param r Shared pointer to movie repository implementation
     * 
     * @pre r must not be null
     * @pre Repository must be properly initialized and connected
     * @post Service is ready for movie viewing operations
     * 
     * @throw std::invalid_argument if repository pointer is null
     * 
     * @note Repository is stored as shared_ptr to enable shared ownership
     * @note Service can be safely copied or moved with repository
     * 
     * Usage:
     * @code
     * auto movieRepo = std::make_shared<MovieRepositorySQL>(connection);
     * auto movieViewer = std::make_unique<MovieViewerService>(movieRepo);
     * 
     * // Service is ready for movie browsing
     * auto movies = movieViewer->showAllMovies();
     * @endcode
     */
    explicit MovieViewerService(std::shared_ptr<IMovieRepository> r);
    
    /**
     * @brief Retrieves all available movies from the catalog
     * 
     * Returns a comprehensive list of all active movies in the cinema system.
     * This method transforms repository data into MovieDTO objects suitable
     * for presentation layer consumption and UI display.
     * 
     * @return Vector of MovieDTO objects containing movie summary information
     * 
     * @post Returns all active movies in the system
     * @post MovieDTO objects contain complete summary information
     * @post Results are ordered appropriately for display
     * 
     * @throw std::runtime_error if repository query fails
     * @throw ServiceUnavailableException if movie service is unavailable
     * 
     * @note Returns only active movies, excluding removed or hidden ones
     * @note Results may be cached for improved performance
     * @note Empty vector returned if no movies are available
     * @note Consider pagination for very large movie catalogs
     * 
     * Implementation Details:
     * - Queries repository for all active movies
     * - Transforms Movie entities to MovieDTO objects
     * - Applies any business rules for movie visibility
     * - May implement caching for frequently accessed data
     * - Handles repository exceptions and provides appropriate errors
     * 
     * Usage:
     * @code
     * try {
     *     auto allMovies = movieViewer->showAllMovies();
     *     
     *     std::cout << "Available Movies (" << allMovies.size() << "):" << std::endl;
     *     for (const auto& movie : allMovies) {
     *         std::cout << "- " << movie.getTitle() 
     *                   << " (" << movie.getDuration() << " min)" << std::endl;
     *     }
     * } catch (const std::exception& e) {
     *     std::cerr << "Failed to load movies: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::vector<MovieDTO> showAllMovies() override;
    
    /**
     * @brief Retrieves detailed information for a specific movie
     * 
     * Returns complete movie details including all properties and metadata
     * for the specified movie. This method provides the full movie entity
     * suitable for detailed viewing pages and booking operations.
     * 
     * @param id Unique identifier of the movie to retrieve
     * @return Shared pointer to the complete movie entity
     * 
     * @pre id must be a valid, existing movie identifier
     * @post Returns complete movie information if found
     * @post Movie object contains all available properties and metadata
     * 
     * @throw std::invalid_argument if id is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw std::runtime_error if repository query fails
     * 
     * @note May return nullptr if movie not found (implementation dependent)
     * @note Movie details include more information than DTO summaries
     * @note Consider caching for frequently accessed movie details
     * 
     * Implementation Details:
     * - Validates movie ID format and range
     * - Queries repository for complete movie entity
     * - Handles movie not found scenarios appropriately
     * - May implement caching for performance optimization
     * - Provides detailed error information for debugging
     * 
     * Usage:
     * @code
     * try {
     *     auto movieDetail = movieViewer->showMovieDetail(selectedMovieId);
     *     
     *     if (movieDetail) {
     *         std::cout << "Movie Details:" << std::endl;
     *         std::cout << "Title: " << movieDetail->getTitle() << std::endl;
     *         std::cout << "Genre: " << movieDetail->getGenre() << std::endl;
     *         std::cout << "Duration: " << movieDetail->getDuration() << " minutes" << std::endl;
     *         std::cout << "Description: " << movieDetail->getDescription() << std::endl;
     *     } else {
     *         std::cout << "Movie not found." << std::endl;
     *     }
     * } catch (const MovieNotFoundException& e) {
     *     std::cout << "Movie does not exist: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::shared_ptr<IMovie> showMovieDetail(int id) override;
    
    /**
     * @brief Retrieves all showtimes for a specific movie
     * 
     * Returns a list of all scheduled showtimes for the specified movie,
     * filtered to show only future/available screenings. This enables
     * users to select their preferred time for ticket booking.
     * 
     * @param id Unique identifier of the movie to query showtimes for
     * @return Vector of ShowTime objects containing complete scheduling information
     * 
     * @pre id must be a valid, existing movie identifier
     * @post Returns all available future showtimes for the movie
     * @post ShowTime objects contain complete scheduling and venue information
     * @post Results are ordered chronologically for easy selection
     * 
     * @throw std::invalid_argument if id is invalid
     * @throw MovieNotFoundException if movie does not exist
     * @throw std::runtime_error if repository query fails
     * 
     * @note Returns only future showtimes, filtering out past screenings
     * @note Empty vector returned if no showtimes are scheduled
     * @note Results include theater, date, time, and availability information
     * @note Consider date range filtering for better performance
     * 
     * Implementation Details:
     * - Validates movie ID and existence
     * - Queries repository for all movie showtimes
     * - Filters showtimes to show only future screenings
     * - Orders results chronologically for user convenience
     * - Includes venue and availability information in results
     * 
     * Usage:
     * @code
     * try {
     *     auto showtimes = movieViewer->showMovieShowTimes(movieId);
     *     
     *     if (!showtimes.empty()) {
     *         std::cout << "Available Showtimes:" << std::endl;
     *         for (const auto& showtime : showtimes) {
     *             std::cout << "- " << showtime.getDateTime() 
     *                       << " (Theater " << showtime.getTheater() 
     *                       << ", " << showtime.getAvailableSeats() 
     *                       << " seats available)" << std::endl;
     *         }
     *     } else {
     *         std::cout << "No showtimes available for this movie." << std::endl;
     *     }
     * } catch (const MovieNotFoundException& e) {
     *     std::cout << "Movie not found: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::vector<ShowTime> showMovieShowTimes(int id) override;  // Changed to return ShowTime vector
};

#endif // MOVIEVIEWERSERVICE_H