/**
 * @file BookingView.h
 * @brief Data Transfer Object for complete booking information display
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef _BOOKINGVIEW_H_
#define _BOOKINGVIEW_H_
#include "../model/ShowTime.h"
#include "../model/ISeat.h"
#include <vector>
#include <string>
#include <memory>

/**
 * @class BookingView
 * @brief Data Transfer Object containing complete booking information for display
 * 
 * This class serves as a comprehensive DTO that aggregates all information
 * related to a booking for presentation purposes. It combines booking details,
 * movie information, showtime data, seat reservations, and pricing into a
 * single convenient object.
 * 
 * @details
 * Key Features:
 * - Complete booking information aggregation
 * - Movie and showtime details integration
 * - Seat reservation and pricing information
 * - Optimized for UI display and reporting
 * 
 * Design Patterns:
 * - **Data Transfer Object (DTO)**: Optimized for data transfer between layers
 * - **Composite**: Aggregates multiple related data entities
 * - **View Object**: Specifically designed for presentation layer consumption
 * - **Value Object**: Immutable data container for booking information
 * 
 * Data Aggregation:
 * - Booking identification and metadata
 * - Movie information (ID, title)
 * - Complete showtime details
 * - Reserved seat collection with details
 * - Total pricing calculation
 * 
 * @par Usage Example
 * @code
 * // Create seat collection
 * std::vector<std::shared_ptr<ISeat>> seats = {
 *     std::make_shared<SingleSeat>("A1", SeatType::SINGLE, 12.99f),
 *     std::make_shared<SingleSeat>("A2", SeatType::SINGLE, 12.99f)
 * };
 * 
 * // Create showtime
 * ShowTime showtime(101, "2025-06-15", "19:30", "22:00");
 * 
 * // Create booking view
 * BookingView view(12345, 67, "Inception", showtime, seats, 25.98f);
 * 
 * // Display booking information
 * std::cout << "Booking #" << view.bookingID << std::endl;
 * std::cout << "Movie: " << view.movieTitle << std::endl;
 * std::cout << "Showtime: " << view.showTime.date << " at " << view.showTime.startTime << std::endl;
 * std::cout << "Seats: " << view.bookedSeats.size() << " seats reserved" << std::endl;
 * std::cout << "Total: $" << view.totalPrice << std::endl;
 * @endcode
 * 
 * @note This class is optimized for read operations and display purposes
 * @warning Do not modify seat objects after creating BookingView
 * 
 * @see ShowTime
 * @see ISeat
 * @see BookingRepository
 * @see IBookingService
 */
class BookingView {
public:
    /**
     * @brief Unique booking identifier
     * 
     * Database primary key that uniquely identifies this booking
     * across the entire system. Used for booking management and references.
     * 
     * @note Should be positive for valid bookings
     */
    int bookingID;
    
    /**
     * @brief Associated movie identifier
     * 
     * Unique identifier of the movie associated with this booking.
     * Links the booking to specific movie content.
     * 
     * @note References Movie entity in the database
     * @see Movie::getId()
     */
    int movieID;
    
    /**
     * @brief Movie title for display
     * 
     * Human-readable title of the movie associated with this booking.
     * Used for user interface display and reporting.
     * 
     * @note Should be non-empty for valid bookings
     * @see Movie::getTitle()
     */
    std::string movieTitle;
    
    /**
     * @brief Complete showtime information
     * 
     * Full showtime details including date, start time, and end time
     * for the specific screening associated with this booking.
     * 
     * @note Contains complete temporal information for the booking
     * @see ShowTime for detailed structure
     */
    ShowTime showTime;
    
    /**
     * @brief Collection of reserved seats
     * 
     * Vector containing all seats that have been reserved as part of
     * this booking. Each seat includes complete seat information.
     * 
     * @note Seats should not be modified after booking creation
     * @see ISeat for seat interface details
     */
    std::vector<std::shared_ptr<ISeat>> bookedSeats;
    
    /**
     * @brief Total booking price
     * 
     * Calculated total cost for this booking including all reserved seats,
     * taxes, fees, and any applicable discounts.
     * 
     * @note Should equal sum of all seat prices plus applicable fees
     * @see ISeat::price() for individual seat pricing
     */
    float totalPrice;

public:
    /**
     * @brief Comprehensive constructor for booking view
     * 
     * Creates a complete booking view with all necessary information
     * for display and processing purposes.
     * 
     * @param bookingID Unique identifier for the booking
     * @param movieID Identifier of the associated movie
     * @param title Title of the movie for display
     * @param showTime Complete showtime information
     * @param bookedSeats Collection of reserved seats
     * @param totalPrice Total cost for the booking
     * 
     * @pre bookingID > 0 (valid booking)
     * @pre movieID > 0 (valid movie)
     * @pre title is non-empty
     * @pre bookedSeats is non-empty for valid bookings
     * @pre totalPrice >= 0.0f
     * @post All booking information is properly initialized
     * 
     * @par Parameter Validation
     * - Booking and movie IDs should be positive
     * - Movie title should be meaningful and non-empty
     * - Showtime should contain valid date/time information
     * - Seat collection should not be empty for paid bookings
     * - Total price should be non-negative
     * 
     * @note Constructor performs deep copy of seat information
     * @see ShowTime constructor for showtime validation
     * @see ISeat for seat object requirements
     * 
     * @par Example
     * @code
     * std::vector<std::shared_ptr<ISeat>> seats;
     * seats.push_back(std::make_shared<SingleSeat>("A1", SeatType::SINGLE, 12.99f));
     * 
     * ShowTime show(101, "2025-06-15", "19:30", "22:00");
     * BookingView view(12345, 67, "Inception", show, seats, 12.99f);
     * @endcode
     */
    BookingView(const int& bookingID, 
                const int& movieID,
                const std::string& title,
                const ShowTime& showTime,
                const std::vector<std::shared_ptr<ISeat>>& bookedSeats,
                const float& totalPrice);
    
    /**
     * @brief Default constructor
     * 
     * Creates an uninitialized booking view. Useful for STL containers
     * and scenarios where booking data will be assigned later.
     * 
     * @post All numeric fields are set to 0/0.0f
     * @post All string fields are empty
     * @post Seat collection is empty
     * 
     * @note Object is in invalid state until properly initialized
     * @warning Do not use uninitialized objects for display
     */
    BookingView() : bookingID(0), movieID(0), totalPrice(0.0f) {}
    
    /**
     * @brief Calculate seat count
     * 
     * Convenience method to get the number of seats in this booking.
     * 
     * @return size_t Number of seats reserved in this booking
     * 
     * @note Useful for display and validation purposes
     */
    size_t getSeatCount() const { return bookedSeats.size(); }
    
    /**
     * @brief Validate booking view data
     * 
     * Checks if the booking view contains valid data for display.
     * 
     * @return bool True if booking view is valid, false otherwise
     * 
     * @note Performs basic validation of critical fields
     */
    bool isValid() const {
        return bookingID > 0 && movieID > 0 && !movieTitle.empty() && 
               !bookedSeats.empty() && totalPrice >= 0.0f;
    }
};

#endif
