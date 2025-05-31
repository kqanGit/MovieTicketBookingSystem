/**
 * @file MovieDTO.h
 * @brief Data Transfer Object for movie information exchange
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef MOVIEDTO_H
#define MOVIEDTO_H
#include <iostream>
#include <memory>       
#include <string>

/**
 * @class MovieDTO
 * @brief Data Transfer Object for efficient movie data transmission
 * 
 * This class serves as a lightweight Data Transfer Object (DTO) for movie
 * information exchange between different layers of the application. It provides
 * a simplified representation of movie data optimized for network transmission,
 * caching, and display purposes.
 * 
 * @details
 * Key Features:
 * - Lightweight movie data representation
 * - Optimized for serialization and transmission
 * - Simplified structure for easy manipulation
 * - Value semantics with comparison support
 * 
 * Design Patterns:
 * - **Data Transfer Object (DTO)**: Optimized for data transfer between layers
 * - **Value Object**: Immutable data container with value semantics
 * - **Plain Old Data (POD)**: Simple structure for easy serialization
 * - **Mapper Pattern**: Used with MovieMapper for entity conversion
 * 
 * Use Cases:
 * - API responses and requests
 * - Cache storage and retrieval
 * - UI data binding and display
 * - Database query result representation
 * - Inter-service communication
 * 
 * @par Usage Example
 * @code
 * // Create movie DTO
 * MovieDTO movie(1, "Inception", "Sci-Fi", 8.8f);
 * 
 * // Use in API response
 * std::vector<MovieDTO> movies = movieService->getAllMovies();
 * 
 * // Serialize for transmission
 * json movieJson = {
 *     {"id", movie.id},
 *     {"title", movie.title},
 *     {"genre", movie.genre},
 *     {"rating", movie.rating}
 * };
 * 
 * // Compare movies
 * MovieDTO other(1, "Inception", "Sci-Fi", 8.8f);
 * if (movie == other) {
 *     std::cout << "Movies are identical" << std::endl;
 * }
 * @endcode
 * 
 * @note DTOs are designed to be lightweight and fast to create/copy
 * @warning DTOs should not contain business logic - use entity classes for that
 * 
 * @see IMovie
 * @see Movie
 * @see MovieMapper
 * @see IMovieRepository
 */
class MovieDTO {
public:
    /**
     * @brief Unique movie identifier
     * 
     * Database primary key that uniquely identifies the movie
     * across the entire system.
     * 
     * @note 0 indicates uninitialized or invalid movie
     */
    int id;
    
    /**
     * @brief Movie title for display
     * 
     * Human-readable title of the movie used for display
     * and user interface purposes.
     * 
     * @note Should be non-empty for valid movies
     */
    std::string title;
    
    /**
     * @brief Movie genre classification
     * 
     * Genre category for the movie used for filtering,
     * categorization, and recommendation purposes.
     * 
     * @note Should follow standard genre classifications
     */
    std::string genre;
    
    /**
     * @brief Movie quality rating
     * 
     * Numeric rating representing movie quality, typically
     * on a scale like 1-10 or similar rating system.
     * 
     * @note 0.0f indicates uninitialized or no rating
     */
    float rating;

    /**
     * @brief Default constructor
     * 
     * Creates an uninitialized movie DTO with default values.
     * Useful for containers and scenarios where data will be assigned later.
     * 
     * @post id == 0 (invalid state)
     * @post rating == 0.0f (no rating)
     * @post title and genre are empty strings
     * 
     * @note Object is in invalid state until properly initialized
     */
    MovieDTO() : id(0), rating(0.0f) {}
    
    /**
     * @brief Parameterized constructor
     * 
     * Creates a movie DTO with all necessary information provided.
     * This is the preferred way to create valid movie DTOs.
     * 
     * @param id Unique movie identifier (should be positive)
     * @param title Movie title (should be non-empty)
     * @param genre Movie genre (should be valid genre)
     * @param rating Movie rating (should be within valid range)
     * 
     * @pre id > 0 for valid movies
     * @pre title is non-empty and meaningful
     * @pre genre follows standard classifications
     * @pre rating is within valid range (e.g., 0.0-10.0)
     * @post All fields are initialized with provided values
     * 
     * @par Example
     * @code
     * MovieDTO movie(1, "Inception", "Sci-Fi", 8.8f);
     * MovieDTO comedy(2, "The Hangover", "Comedy", 7.7f);
     * @endcode
     */
    MovieDTO(int id, const std::string& title, const std::string& genre, float rating);
    
    /**
     * @brief Equality comparison operator
     * 
     * Compares two movie DTOs for complete equality across all fields.
     * Useful for testing, caching, and duplicate detection.
     * 
     * @param other Movie DTO to compare against
     * @return bool True if all fields are equal, false otherwise
     * 
     * @note Compares all fields: id, title, genre, and rating
     * @note Floating-point rating comparison uses exact equality
     * 
     * @par Example
     * @code
     * MovieDTO movie1(1, "Inception", "Sci-Fi", 8.8f);
     * MovieDTO movie2(1, "Inception", "Sci-Fi", 8.8f);
     * MovieDTO movie3(2, "Avatar", "Action", 7.9f);
     * 
     * assert(movie1 == movie2);  // true - identical content
     * assert(!(movie1 == movie3)); // false - different content
     * @endcode
     */
    bool operator==(const MovieDTO& other) const {
        return id == other.id && title == other.title 
            && genre == other.genre && rating == other.rating;
    }
    
    /**
     * @brief Inequality comparison operator
     * 
     * Compares two movie DTOs for inequality. Convenience operator
     * that negates the equality comparison.
     * 
     * @param other Movie DTO to compare against
     * @return bool True if any field differs, false if all equal
     * 
     * @note Implemented as negation of operator==
     */
    bool operator!=(const MovieDTO& other) const {
        return !(*this == other);
    }
    
    /**
     * @brief Check if DTO contains valid data
     * 
     * Validates that the DTO contains meaningful data that can be
     * used for business operations.
     * 
     * @return bool True if DTO is valid, false otherwise
     * 
     * @note Checks for positive ID, non-empty title, and valid rating
     */
    bool isValid() const {
        return id > 0 && !title.empty() && rating >= 0.0f;
    }
    
    /**
     * @brief Convert to string representation
     * 
     * Creates a human-readable string representation of the movie DTO
     * for debugging and logging purposes.
     * 
     * @return std::string String representation of the movie
     * 
     * @par Example Output
     * "Movie[ID=1, Title='Inception', Genre='Sci-Fi', Rating=8.8]"
     */
    std::string toString() const {
        return "Movie[ID=" + std::to_string(id) + 
               ", Title='" + title + 
               "', Genre='" + genre + 
               "', Rating=" + std::to_string(rating) + "]";
    }
};

#endif // MOVIEDTO_H