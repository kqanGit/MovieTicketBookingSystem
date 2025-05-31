/**
 * @file IMovie.h
 * @brief Abstract interface for movie entities in the booking system
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef IMOVIE_H
#define IMOVIE_H

#include <iostream>
#include <string>

/**
 * @interface IMovie
 * @brief Abstract interface defining the contract for movie entities
 * 
 * This interface establishes the standard contract that all movie implementations
 * must follow. It defines the essential properties and operations required for
 * movie management in the booking system.
 * 
 * @details
 * Key Responsibilities:
 * - Define movie property access interface
 * - Establish consistent movie data structure
 * - Enable polymorphic movie handling
 * - Support different movie implementations
 * 
 * Design Patterns:
 * - **Interface Segregation**: Focused interface for movie operations
 * - **Strategy Pattern**: Enables different movie implementation strategies
 * - **Template Method**: Defines template for movie property management
 * - **Polymorphism**: Allows treating different movie types uniformly
 * 
 * Movie Properties:
 * - Unique identification (ID)
 * - Descriptive information (title, genre, description)
 * - Quality metrics (rating)
 * - Metadata for system management
 * 
 * @par Usage Example
 * @code
 * class ConcreteMovie : public IMovie {
 *     // Implement all pure virtual methods
 * };
 * 
 * std::unique_ptr<IMovie> movie = std::make_unique<ConcreteMovie>();
 * movie->setTitle("Inception");
 * movie->setGenre("Sci-Fi");
 * movie->setRating(8.8f);
 * 
 * std::cout << "Movie: " << movie->getTitle() 
 *           << " (" << movie->getGenre() << ")" << std::endl;
 * @endcode
 * 
 * @note All implementing classes must provide concrete implementations
 * @warning Ensure proper validation in setter implementations
 * 
 * @see Movie
 * @see IMovieRepository
 * @see MovieService
 */
class IMovie {
public:
    /**
     * @brief Virtual destructor for proper polymorphic destruction
     * 
     * Ensures that derived class destructors are called correctly
     * when deleting through base class pointers.
     * 
     * @note Essential for proper resource cleanup in inheritance hierarchies
     */
    virtual ~IMovie() {}
    
    // ============================================================================
    // GETTER METHODS
    // ============================================================================
    
    /**
     * @brief Get the movie's unique identifier
     * 
     * Returns the unique database identifier for this movie.
     * Used for database operations and movie lookups.
     * 
     * @return int The unique movie ID
     * 
     * @post Returns consistent ID for the same movie instance
     * 
     * @note ID should be unique across the entire movie database
     * @see setId()
     */
    virtual int getId() const = 0;
    
    /**
     * @brief Get the movie title
     * 
     * Returns the full title of the movie as displayed to users.
     * This is the primary identifier users will see.
     * 
     * @return std::string The movie title
     * 
     * @post Returns non-empty string for valid movies
     * 
     * @note Title should be user-friendly and searchable
     * @see setTitle()
     */
    virtual std::string getTitle() const = 0;
    
    /**
     * @brief Get the movie genre
     * 
     * Returns the primary genre classification for this movie.
     * Used for categorization, filtering, and recommendations.
     * 
     * @return std::string The movie genre (e.g., "Action", "Comedy", "Drama")
     * 
     * @note Should follow standard genre classifications
     * @see setGenre()
     */
    virtual std::string getGenre() const = 0;
    
    /**
     * @brief Get the movie description
     * 
     * Returns a detailed description or synopsis of the movie.
     * Provides users with plot information to aid in selection.
     * 
     * @return std::string Detailed movie description/synopsis
     * 
     * @note Description should be informative but spoiler-free
     * @see setDescription()
     */
    virtual std::string getDescription() const = 0;
    
    /**
     * @brief Get the movie rating
     * 
     * Returns the movie's rating score, typically on a scale like 1-10.
     * Used for quality indication and recommendation algorithms.
     * 
     * @return float The movie rating (e.g., 8.5 out of 10)
     * 
     * @post Returns value within valid rating range
     * 
     * @note Rating scale should be consistent across the system
     * @see setRating()
     */
    virtual float getRating() const = 0;

    // ============================================================================
    // SETTER METHODS
    // ============================================================================
    
    /**
     * @brief Set the movie's unique identifier
     * 
     * Assigns a unique identifier to this movie instance.
     * Typically used during database persistence operations.
     * 
     * @param id Unique identifier for the movie
     * 
     * @pre id should be positive for valid movies
     * @post getId() returns the assigned id
     * 
     * @note ID assignment should be handled by database/repository layers
     * @warning Changing ID after persistence may cause data inconsistencies
     * @see getId()
     */
    virtual void setId(int id) = 0;
    
    /**
     * @brief Set the movie title
     * 
     * Assigns the display title for this movie.
     * Title should be unique and user-friendly.
     * 
     * @param title The movie title to assign
     * 
     * @pre title should be non-empty
     * @post getTitle() returns the assigned title
     * 
     * @throws std::invalid_argument If title is empty or invalid
     * 
     * @note Title should be validated for appropriate length and content
     * @see getTitle()
     */
    virtual void setTitle(const std::string& title) = 0;
    
    /**
     * @brief Set the movie genre
     * 
     * Assigns the genre classification for this movie.
     * Should follow standard genre categories.
     * 
     * @param genre The genre to assign (e.g., "Action", "Comedy")
     * 
     * @pre genre should be non-empty and from valid genre list
     * @post getGenre() returns the assigned genre
     * 
     * @note Consider validating against predefined genre list
     * @see getGenre()
     */
    virtual void setGenre(const std::string& genre) = 0;
    
    /**
     * @brief Set the movie description
     * 
     * Assigns a detailed description or synopsis for this movie.
     * Should provide helpful information without spoilers.
     * 
     * @param description The description text to assign
     * 
     * @pre description should be non-empty and informative
     * @post getDescription() returns the assigned description
     * 
     * @note Consider length limits for database storage
     * @see getDescription()
     */
    virtual void setDescription(const std::string& description) = 0;
    
    /**
     * @brief Set the movie rating
     * 
     * Assigns a quality rating score for this movie.
     * Should be within the valid rating range for the system.
     * 
     * @param rating The rating score to assign
     * 
     * @pre rating should be within valid range (e.g., 0.0-10.0)
     * @post getRating() returns the assigned rating
     * 
     * @throws std::out_of_range If rating is outside valid range
     * 
     * @note Rating scale should be clearly documented and consistent
     * @see getRating()
     */
    virtual void setRating(float rating) = 0;
};
    
#endif //IMOVIE_H