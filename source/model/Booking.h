/**
 * @file Booking.h
 * @brief Data model representing a movie ticket booking
 * @author Movie Ticket Booking System Team
 */

#ifndef _BOOKING_H_
#define _BOOKING_H_
#include <string>
#include "ShowTime.h"

/**
 * @struct Booking
 * @brief Represents a movie ticket booking record
 * 
 * This structure encapsulates all essential information about a booking
 * made by a user, including movie details, showtime, and booking identification.
 * 
 * @details
 * The Booking struct follows the Data Transfer Object (DTO) pattern,
 * providing a simple container for booking data that can be easily
 * passed between layers of the application.
 * 
 * Key responsibilities:
 * - Store booking identification and metadata
 * - Link bookings to specific movies and showtimes
 * - Provide immutable booking record structure
 * - Support booking history and reporting features
 * 
 * @note This is a value type designed for efficient copying and storage
 * @see ShowTime
 * @see BookingView
 * @since v1.0
 */
struct Booking {
    /**
     * @brief Unique identifier for this booking
     * 
     * System-generated unique ID that serves as the primary key
     * for booking records in the database.
     */
    int bookingID;
    
    /**
     * @brief ID of the booked movie
     * 
     * Foreign key reference to the Movie entity, enabling
     * retrieval of complete movie information.
     */
    int movieID;
    
    /**
     * @brief Title of the booked movie
     * 
     * Denormalized movie title for quick access without
     * requiring joins to the Movie table.
     */
    std::string movieTitle;
    
    /**
     * @brief Showtime details for this booking
     * 
     * Complete showtime information including date, time,
     * theater, and seating arrangement details.
     */
    ShowTime showTime;
    
    /**
     * @brief Constructs a Booking with all required information
     * 
     * Creates a new booking record with the specified details.
     * Used primarily for loading existing bookings from storage.
     * 
     * @param bookingID Unique booking identifier
     * @param movieID Movie identifier for the booking
     * @param movieTitle Title of the booked movie
     * @param showTime Showtime details for the booking
     * 
     * @pre bookingID must be positive and unique
     * @pre movieID must reference a valid movie
     * @pre movieTitle must not be empty
     * @pre showTime must be valid and in the future (for new bookings)
     * 
     * @post Booking instance is fully initialized and ready for use
     * 
     * @note Constructor performs basic validation of input parameters
     * 
     * @see ShowTime
     * @since v1.0
     */
    Booking(
        const int& bookingID,
        const int& movieID,
        const std::string& movieTitle,
        const ShowTime& showTime
    );
};
#endif
