/**
 * @file Movie.h
 * @brief Movie entity model class
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef MOVIE_H
#define MOVIE_H

#include "IMovie.h"
#include <vector>
#include <string>

/**
 * @class Movie
 * @brief Concrete implementation of movie entity in the booking system
 * 
 * Represents a movie with all its metadata including basic information,
 * ratings, and associated show times. This class implements the IMovie
 * interface and serves as the primary movie data model.
 * 
 * @details
 * The Movie class encapsulates:
 * - Basic movie information (title, genre, description)
 * - Rating system integration
 * - Show time management
 * - Unique identification
 * 
 * @par Design Patterns Used
 * - Entity Pattern: Represents a domain object with identity
 * - Data Transfer Object: Transfers movie data between layers
 * 
 * @implements IMovie
 * 
 * @invariant id >= 0 (0 for new movies, positive for persisted movies)
 * @invariant rating >= 0.0 && rating <= 10.0
 * @invariant !title.empty()
 */
class Movie : public IMovie
{
private:
    /**
     * @brief Unique identifier for the movie
     * 
     * Database primary key for the movie. Value of 0 indicates
     * a new movie that hasn't been persisted to database yet.
     */
    int id;
    
    /**
     * @brief Movie title/name
     * 
     * The display name of the movie as shown to users.
     * Must not be empty for valid movie instances.
     */
    std::string title;
    
    /**
     * @brief Movie genre classification
     * 
     * Genre category such as "Action", "Comedy", "Drama", "Horror", etc.
     * Used for filtering and categorization in the UI.
     */
    std::string genre;
    
    /**
     * @brief Detailed movie description/synopsis
     * 
     * Extended description providing plot summary, cast information,
     * and other relevant details about the movie.
     */
    std::string description;
    
    /**
     * @brief Movie rating score
     * 
     * Numeric rating typically on a scale of 0.0 to 10.0.
     * Represents average user/critic rating for the movie.
     */
    float rating;
    
    /**
     * @brief List of available show times
     * 
     * Collection of time slots when this movie is scheduled to be shown.
     * Times are typically stored in "HH:MM" format.
     */
    std::vector<std::string> showTimes;

public:
    /**
     * @brief Default constructor
     * 
     * Creates an empty movie object with default values.
     * Primarily used for object initialization before setting actual values.
     * 
     * @post id == 0
     * @post title.empty() == true
     * @post rating == 0.0
     * @post showTimes.empty() == true
     */
    Movie() {}
    
    /**
     * @brief Constructor with full movie information including ID
     * 
     * Creates a movie object with all properties specified, typically
     * used when loading movies from database (with existing ID).
     * 
     * @param id Unique movie identifier
     * @param title Movie title/name
     * @param genre Movie genre classification
     * @param description Detailed movie description
     * @param rating Movie rating score (0.0-10.0)
     * 
     * @pre !title.empty()
     * @pre rating >= 0.0 && rating <= 10.0
     * @post getId() == id
     * @post getTitle() == title
     */
    Movie(int id, const std::string &title, const std::string &genre,
          const std::string &description, float rating);
    
    /**
     * @brief Constructor for new movies without ID
     * 
     * Creates a new movie object without specifying ID, typically
     * used when creating new movies before database insertion.
     * 
     * @param title Movie title/name
     * @param genre Movie genre classification
     * @param description Detailed movie description
     * @param rating Movie rating score (0.0-10.0)
     * 
     * @pre !title.empty()
     * @pre rating >= 0.0 && rating <= 10.0
     * @post getId() == 0
     * @post getTitle() == title
     */
    Movie(const std::string &title, const std::string &genre,
          const std::string &description, float rating);

    // IMovie interface implementation
    
    /**
     * @brief Get movie unique identifier
     * @return int Movie ID (0 for new movies, positive for persisted movies)
     * @override
     */
    int getId() const override;
    
    /**
     * @brief Get movie title
     * @return std::string Movie title/name
     * @override
     */
    std::string getTitle() const override;
    
    /**
     * @brief Get movie genre
     * @return std::string Genre classification
     * @override
     */
    std::string getGenre() const override;
    
    /**
     * @brief Get movie description
     * @return std::string Detailed description/synopsis
     * @override
     */
    std::string getDescription() const override;
    
    /**
     * @brief Get movie rating
     * @return float Rating score (0.0-10.0)
     * @override
     */
    float getRating() const override;

    // Additional movie-specific methods
    
    /**
     * @brief Get all show times for this movie
     * 
     * @return const std::vector<std::string>& Reference to show times collection
     * 
     * @note Returned reference is valid until next modification to showTimes
     */
    const std::vector<std::string> &getShowTimes() const;
    
    /**
     * @brief Add a new show time for this movie
     * 
     * @param time Show time in "HH:MM" format
     * 
     * @pre !time.empty()
     * @post getShowTimes().size() increased by 1
     * @post Last element of getShowTimes() == time
     */
    void addShowTime(const std::string &time);

    // Setter methods for movie properties
    
    /**
     * @brief Set movie ID
     * @param _id New movie identifier
     * @post getId() == _id
     */
    void setId(int _id) { id = _id; }
    
    /**
     * @brief Set movie title
     * @param newTitle New movie title
     * @pre !newTitle.empty()
     * @post getTitle() == newTitle
     */
    void setTitle(const std::string &newTitle) { title = newTitle; }
    
    /**
     * @brief Set movie genre
     * @param newGenre New genre classification
     * @post getGenre() == newGenre
     */
    void setGenre(const std::string &newGenre) { genre = newGenre; }
    
    /**
     * @brief Set movie rating
     * @param newRating New rating score
     * @pre newRating >= 0.0 && newRating <= 10.0
     * @post getRating() == newRating
     */
    void setRating(float newRating) { rating = newRating; }
    
    /**
     * @brief Set movie description
     * @param newDescription New detailed description
     * @post getDescription() == newDescription
     */
    void setDescription(const std::string &newDescription) { description = newDescription; }
};

#endif // MOVIE_H