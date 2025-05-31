/**
 * @file IMovieRepository.h
 * @brief Repository interface for movie data access operations
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef IMOVIEREPOSITORY_H
#define IMOVIEREPOSITORY_H

#include <vector>
#include "MovieDTO.h"
#include "../model/IMovie.h"
#include "../model/ShowTime.h"
#include <memory>
#include <string>
#include <iostream>

/**
 * @interface IMovieRepository
 * @brief Repository interface for movie and showtime data access operations
 * 
 * This interface defines the contract for movie data persistence and retrieval
 * operations. It implements the Repository pattern to abstract data access
 * logic from business logic, enabling different storage implementations.
 * 
 * @details
 * The IMovieRepository provides:
 * - CRUD operations for movies
 * - Showtime management per movie
 * - Data transfer object support
 * - Abstract persistence layer
 * 
 * @par Design Patterns Used
 * - Repository Pattern: Abstracts data access implementation
 * - Data Transfer Object: Uses MovieDTO for data exchange
 * - Interface Segregation: Focused on movie-related operations only
 * 
 * @par Concrete Implementations
 * - MovieRepositorySQL: SQLite database implementation
 * - MovieRepositoryMock: In-memory mock for testing
 * 
 * @see MovieDTO
 * @see IMovie
 * @see ShowTime
 */
class IMovieRepository {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~IMovieRepository() {}
    
    /**
     * @brief Retrieve all movies from the repository
     * 
     * Fetches all movies currently stored in the repository,
     * returning them as Data Transfer Objects for efficient transmission.
     * 
     * @return std::vector<MovieDTO> Collection of all movies as DTOs
     * 
     * @retval Empty vector if no movies exist
     * @retval Vector of MovieDTO objects containing movie data
     * 
     * @note MovieDTO provides a lightweight representation suitable
     *       for display and transfer purposes
     * 
     * @see MovieDTO
     */
    virtual std::vector<MovieDTO> getAllMovies() = 0;
    
    /**
     * @brief Retrieve a specific movie by its unique identifier
     * 
     * @param id Unique movie identifier
     * @return std::shared_ptr<IMovie> Smart pointer to movie object or nullptr
     * 
     * @retval nullptr if movie with given ID doesn't exist
     * @retval Valid IMovie pointer if movie found
     * 
     * @pre id > 0
     * 
     * @see IMovie
     */
    virtual std::shared_ptr<IMovie> getMovieById(int id) = 0;
    
    /**
     * @brief Add a new movie to the repository
     * 
     * Persists a new movie to the underlying storage and assigns
     * a unique identifier if successful.
     * 
     * @param movie Smart pointer to movie object to be added
     * @return int Generated movie ID on success, -1 on failure
     * 
     * @pre movie != nullptr
     * @pre movie->getTitle() is not empty
     * @post If successful: returned ID > 0 and movie is persisted
     * @post If failed: returned value == -1
     * 
     * @throws RepositoryException if persistence operation fails
     * 
     * @see IMovie
     */
    virtual int addMovie(std::shared_ptr<IMovie> movie) = 0;
    
    /**
     * @brief Remove a movie from the repository
     * 
     * Deletes the movie with the specified ID and all associated
     * data including showtimes and bookings.
     * 
     * @param id Unique identifier of movie to delete
     * 
     * @pre id > 0
     * @post Movie and all related data are permanently removed
     * 
     * @warning This operation cascades to delete related showtimes
     * 
     * @see deleteAllShowTimes()
     */
    virtual void deleteMovie(int id) = 0;
    
    /**
     * @brief Add a new showtime for a specific movie
     * 
     * Creates a new showtime entry associating a time slot with a movie.
     * The showtime defines when the movie will be screened.
     * 
     * @param movieId ID of the movie for this showtime
     * @param Date Screening date in YYYY-MM-DD format
     * @param StartTime Start time in HH:MM format
     * @param EndTime End time in HH:MM format
     * 
     * @pre movieId > 0 and movie exists
     * @pre Date is valid and not in the past
     * @pre StartTime < EndTime
     * @post New showtime is created and associated with movie
     * 
     * @throws InvalidDateException if date format is invalid
     * @throws RepositoryException if movie doesn't exist
     * 
     * @see ShowTime
     */
    virtual void addShowTime(int movieId, std::string& Date, std::string& StartTime, std::string& EndTime) = 0;
    
    /**
     * @brief Remove a specific showtime
     * 
     * Deletes a particular showtime entry, making those seats
     * unavailable for booking.
     * 
     * @param movieId ID of the movie
     * @param ShowTimeId ID of the showtime to delete
     * 
     * @pre movieId > 0 and ShowTimeId > 0
     * @post Showtime is removed and associated bookings are handled
     * 
     * @warning May affect existing bookings for this showtime
     */
    virtual void deleteShowTime(int movieId, int ShowTimeId) = 0;
    
    /**
     * @brief Get all showtimes for a specific movie
     * 
     * Retrieves all scheduled screening times for the specified movie,
     * including date, start time, and end time information.
     * 
     * @param id Movie unique identifier
     * @return std::vector<ShowTime> Collection of showtime objects
     * 
     * @retval Empty vector if movie has no showtimes
     * @retval Vector of ShowTime objects with scheduling details
     * 
     * @pre id > 0
     * 
     * @see ShowTime
     */
    virtual std::vector<ShowTime> getShowTimesByMovieId(int id) = 0;
    
    /**
     * @brief Remove all showtimes for a specific movie
     * 
     * Bulk operation to delete all scheduled showtimes for a movie.
     * Typically used when removing a movie from the system.
     * 
     * @param movieId ID of the movie to clear showtimes for
     * 
     * @pre movieId > 0
     * @post All showtimes for the movie are removed
     * 
     * @warning This affects all future bookings for this movie
     * 
     * @see deleteMovie()
     */
    virtual void deleteAllShowTimes(int movieId) = 0;
};

#endif // IMOVIEREPOSITORY_H