/**
 * @file MovieRepositorySQL.h
 * @brief SQLite implementation of movie repository interface
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef MOVIEREPOSITORYSQL_H
#define MOVIEREPOSITORYSQL_H

#include "../repository/IMovieRepository.h"
#include "../database/DatabaseConnection.h"
#include "../model/ShowTime.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <regex>

/**
 * @class MovieRepositorySQL
 * @brief SQLite database implementation of movie data access layer
 * 
 * This class provides concrete implementation of movie data persistence
 * using SQLite database. It handles all movie-related database operations
 * including CRUD operations for movies and showtime management.
 * 
 * @details
 * Key Features:
 * - Complete movie CRUD operations
 * - Showtime scheduling and management
 * - Data validation and integrity checking
 * - Automatic database schema management
 * - SQL injection prevention through parameterized queries
 * - Transaction support for atomic operations
 * 
 * @par Design Patterns Used
 * - Repository Pattern: Implements IMovieRepository interface
 * - Singleton Pattern: Uses DatabaseConnection singleton
 * - DTO Pattern: Returns MovieDTO objects for data transfer
 * - RAII: Automatic resource management in destructor
 * 
 * @par Database Schema Requirements
 * This repository expects the following tables:
 * - movies: Movie information storage
 * - showtimes: Movie scheduling information
 * - theaters: Theater and seat information
 * 
 * @par Usage Example
 * @code
 * MovieRepositorySQL repository("database.db");
 * 
 * // Get all movies
 * auto movies = repository.getAllMovies();
 * 
 * // Add new movie
 * auto movie = std::make_shared<Movie>("Inception", "Sci-Fi", 148);
 * int movieId = repository.addMovie(movie);
 * 
 * // Add showtime
 * std::string date = "2025-06-01";
 * std::string startTime = "19:30";
 * std::string endTime = "22:00";
 * repository.addShowTime(movieId, date, startTime, endTime);
 * @endcode
 * 
 * @warning Database connection must remain valid throughout repository lifetime
 * 
 * @see IMovieRepository
 * @see DatabaseConnection
 * @see MovieDTO
 * @see ShowTime
 */
class MovieRepositorySQL : public IMovieRepository {
private:
    /**
     * @brief Database connection instance
     * 
     * Pointer to the singleton database connection used for all
     * SQL operations. Managed by DatabaseConnection class lifecycle.
     */
    DatabaseConnection* dbConn;

public:
    /**
     * @brief Constructor with database file path
     * 
     * Initializes the repository with a specific SQLite database file.
     * Establishes connection and prepares database schema if needed.
     * 
     * @param filePath Path to the SQLite database file
     * 
     * @pre filePath is a valid file path
     * @post dbConn != nullptr and connection is established
     * 
     * @throws std::runtime_error if database connection fails
     * @throws std::invalid_argument if filePath is empty
     * 
     * @par Database Initialization
     * If the database file doesn't exist, it will be created with
     * the required schema for movie management.
     */
    explicit MovieRepositorySQL(const std::string& filePath);

    /**
     * @brief Retrieve all movies from database
     * 
     * Fetches complete list of all movies stored in the database
     * with basic information suitable for listing and selection.
     * 
     * @return std::vector<MovieDTO> Vector of movie data transfer objects
     * 
     * @retval Empty vector if no movies exist
     * @retval Vector of MovieDTO with movie information
     * 
     * @post Return value contains all movies in database
     * 
     * @par Performance Considerations
     * For large movie databases, consider implementing pagination
     * or filtering mechanisms to limit returned results.
     * 
     * @see MovieDTO for returned data structure
     */
    std::vector<MovieDTO> getAllMovies() override;

    /**
     * @brief Retrieve specific movie by ID
     * 
     * Fetches detailed information for a single movie including
     * all associated metadata and relationships.
     * 
     * @param id Unique movie identifier
     * @return std::shared_ptr<IMovie> Shared pointer to movie object
     * 
     * @retval Valid movie object if found
     * @retval nullptr if movie doesn't exist
     * 
     * @pre id > 0 (valid movie identifier)
     * 
     * @see IMovie interface for returned object capabilities
     */
    std::shared_ptr<IMovie> getMovieById(int id) override;

    /**
     * @brief Add new movie to database
     * 
     * Inserts a new movie record into the database with complete
     * validation and returns the generated movie ID.
     * 
     * @param movie Shared pointer to movie object to add
     * @return int Generated unique movie ID
     * 
     * @retval Positive integer representing new movie ID
     * @retval -1 if insertion failed
     * 
     * @pre movie != nullptr
     * @pre Movie contains valid data (title, genre, duration)
     * 
     * @post Movie record created in database
     * @post Generated ID assigned to movie
     * 
     * @throws std::invalid_argument if movie data is invalid
     * @throws std::runtime_error if database operation fails
     */
    int addMovie(std::shared_ptr<IMovie> movie) override;

    /**
     * @brief Delete movie from database
     * 
     * Removes movie record and all associated data (showtimes, bookings)
     * from the database. This operation is irreversible.
     * 
     * @param id Unique movie identifier to delete
     * 
     * @pre id > 0 (valid movie identifier)
     * 
     * @post Movie and all related data removed from database
     * @post All associated showtimes cancelled
     * 
     * @warning This operation cascades to related bookings and showtimes
     * 
     * @see deleteAllShowTimes() for showtime-specific deletion
     */
    void deleteMovie(int id) override;

    /**
     * @brief Add showtime for a movie
     * 
     * Creates a new showtime schedule for an existing movie with
     * date and time validation.
     * 
     * @param movieId ID of the movie to schedule
     * @param Date Show date in YYYY-MM-DD format
     * @param StartTime Start time in HH:MM format
     * @param EndTime End time in HH:MM format
     * 
     * @pre movieId > 0 and movie exists
     * @pre Date is valid and in future
     * @pre StartTime and EndTime are valid and StartTime < EndTime
     * 
     * @post New showtime record created
     * @post Seats initialized for the showtime
     * 
     * @throws std::invalid_argument if date/time format is invalid
     * @throws std::runtime_error if movie doesn't exist
     * 
     * @see isValidDate() and isValidTime() for validation rules
     */
    void addShowTime(int movieId, std::string& Date, std::string& StartTime, std::string& EndTime) override;

    /**
     * @brief Delete specific showtime
     * 
     * Removes a specific showtime for a movie, cancelling any
     * existing bookings for that show.
     * 
     * @param movieId ID of the movie
     * @param ShowTimeID ID of the showtime to delete
     * 
     * @pre movieId > 0 and ShowTimeID > 0
     * @pre Showtime exists for the specified movie
     * 
     * @post Showtime record removed
     * @post Associated bookings cancelled
     * 
     * @warning Existing bookings for this showtime will be cancelled
     */
    void deleteShowTime(int movieId, int ShowTimeID) override;

    /**
     * @brief Get all showtimes for a movie
     * 
     * Retrieves complete schedule information for a specific movie
     * including available and past showtimes.
     * 
     * @param id Movie ID to get showtimes for
     * @return std::vector<ShowTime> Vector of showtime objects
     * 
     * @retval Empty vector if no showtimes exist
     * @retval Vector of ShowTime objects with schedule details
     * 
     * @pre id > 0 (valid movie identifier)
     * 
     * @see ShowTime for returned object structure
     */
    std::vector<ShowTime> getShowTimesByMovieId(int id) override;

    /**
     * @brief Delete all showtimes for a movie
     * 
     * Removes all scheduled showtimes for a specific movie.
     * Typically used when removing a movie from circulation.
     * 
     * @param movieId ID of the movie
     * 
     * @pre movieId > 0 (valid movie identifier)
     * 
     * @post All showtimes for movie removed
     * @post All associated bookings cancelled
     * 
     * @warning This cancels all bookings for the movie
     */
    void deleteAllShowTimes(int movieId) override;

    /**
     * @brief Destructor - cleanup database resources
     * 
     * Ensures proper cleanup of database connections and resources.
     * DatabaseConnection singleton manages actual connection lifecycle.
     */
    ~MovieRepositorySQL();
};

/**
 * @brief Validate date format and value
 * 
 * Checks if a date string is in valid YYYY-MM-DD format
 * and represents a real calendar date.
 * 
 * @param date Date string to validate
 * @return bool True if date is valid, false otherwise
 * 
 * @par Validation Rules
 * - Format: YYYY-MM-DD
 * - Year: 1900-2100
 * - Month: 01-12
 * - Day: Valid for the month/year
 * 
 * @see addShowTime() which uses this validation
 */
bool isValidDate(const std::string &date);

/**
 * @brief Validate time format
 * 
 * Checks if a time string is in valid HH:MM format
 * with proper hour and minute ranges.
 * 
 * @param time Time string to validate
 * @return bool True if time is valid, false otherwise
 * 
 * @par Validation Rules
 * - Format: HH:MM
 * - Hours: 00-23
 * - Minutes: 00-59
 * 
 * @see addShowTime() which uses this validation
 */
bool isValidTime(const std::string &time);

#endif // MOVIEREPOSITORYSQL_H