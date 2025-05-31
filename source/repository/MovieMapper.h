/**
 * @file MovieMapper.h
 * @brief Movie object-relational mapping utility
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef MOVIEMAPPER_H
#define MOVIEMAPPER_H

#include "../model/IMovie.h"
#include "MovieDTO.h"
#include <memory>

/**
 * @class MovieMapper
 * @brief Provides mapping functionality between Movie entities and MovieDTO objects
 * 
 * The MovieMapper class implements the Data Mapper pattern, providing static methods
 * for converting between domain objects (IMovie) and data transfer objects (MovieDTO).
 * This separation allows for clean data transformation between the business layer
 * and persistence layer while maintaining proper abstraction.
 * 
 * @details Key Features:
 * - Bidirectional mapping between entities and DTOs
 * - Static utility methods for clean API usage
 * - Thread-safe operations (stateless design)
 * - Null-safe implementations
 * 
 * @note This class follows the static utility pattern - all methods are static
 *       and the class cannot be instantiated.
 * 
 * Usage Example:
 * @code
 * // Convert entity to DTO for database operations
 * std::shared_ptr<IMovie> movie = getMovieFromSomewhere();
 * MovieDTO dto = MovieMapper::toDTO(movie);
 * 
 * // Convert DTO back to entity after database retrieval
 * MovieDTO retrievedDto = database.getMovie(id);
 * auto entity = MovieMapper::toEntity(retrievedDto);
 * 
 * // Update existing entity with DTO data
 * MovieMapper::updateFromDTO(existingMovie, updatedDto);
 * @endcode
 * 
 * @see MovieDTO
 * @see IMovie
 * @since 1.0
 */
class MovieMapper {
public:
    /**
     * @brief Converts an IMovie entity to a MovieDTO object
     * 
     * Transforms a domain movie object into a data transfer object suitable
     * for database operations or serialization. All movie properties are
     * mapped to their corresponding DTO fields.
     * 
     * @param movie Shared pointer to the movie entity to convert
     * @return MovieDTO object containing the mapped data
     * 
     * @pre movie must not be null
     * @post Returned DTO contains all data from the input movie
     * 
     * @throw std::invalid_argument if movie pointer is null
     * 
     * @note This method is thread-safe and does not modify the input entity
     * 
     * Usage:
     * @code
     * std::shared_ptr<IMovie> movie = std::make_shared<Movie>("Inception", 148, "Sci-Fi");
     * MovieDTO dto = MovieMapper::toDTO(movie);
     * // dto now contains all movie data for persistence operations
     * @endcode
     */
    static MovieDTO toDTO(const std::shared_ptr<IMovie>& movie);
    
    /**
     * @brief Creates an IMovie entity from a MovieDTO object
     * 
     * Constructs a new movie domain object from the provided DTO data.
     * This method creates a concrete Movie implementation that implements
     * the IMovie interface.
     * 
     * @param dto The MovieDTO containing the source data
     * @return Shared pointer to the newly created movie entity
     * 
     * @pre dto must contain valid movie data
     * @post Returns a valid IMovie implementation with DTO data
     * 
     * @throw std::invalid_argument if DTO contains invalid data
     * 
     * @note Creates a new Movie instance - does not reuse existing objects
     * @note The returned object is completely independent of the input DTO
     * 
     * Usage:
     * @code
     * MovieDTO dto("Avatar", 162, "Action");
     * auto movie = MovieMapper::toEntity(dto);
     * // movie is now a fully functional IMovie object
     * @endcode
     */
    static std::shared_ptr<IMovie> toEntity(const MovieDTO& dto);
    
    /**
     * @brief Updates an existing IMovie entity with data from a MovieDTO
     * 
     * Modifies the properties of an existing movie object using the values
     * from the provided DTO. This is useful for updating entities with
     * new data from the database without creating new objects.
     * 
     * @param movie Shared pointer to the movie entity to update
     * @param dto The MovieDTO containing the new data
     * 
     * @pre movie must not be null
     * @pre dto must contain valid movie data
     * @post movie contains all updated data from dto
     * 
     * @throw std::invalid_argument if movie is null or dto is invalid
     * 
     * @note This method modifies the existing movie object in-place
     * @note Thread safety depends on the underlying movie implementation
     * 
     * Usage:
     * @code
     * std::shared_ptr<IMovie> movie = getExistingMovie();
     * MovieDTO updatedData("New Title", 120, "Drama");
     * MovieMapper::updateFromDTO(movie, updatedData);
     * // movie now has the updated properties
     * @endcode
     */
    static void updateFromDTO(std::shared_ptr<IMovie> movie, const MovieDTO& dto);
};

#endif // MOVIEMAPPER_H