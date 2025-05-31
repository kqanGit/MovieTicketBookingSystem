
/**
 * @file IBookingService.h
 * @brief Interface for movie ticket booking operations
 * @author Movie Ticket Booking System Team
 */

#ifndef _IBOOKINGSERVICE_H_
#define _IBOOKINGSERVICE_H_
#include <string>
#include <vector>
#include "../repository/SeatView.h"
#include "../repository/BookingView.h"

/**
 * @interface IBookingService
 * @brief Interface defining booking operations for movie tickets
 * 
 * This interface provides a contract for all booking-related operations,
 * implementing the Service Layer pattern to encapsulate business logic
 * for ticket reservations and seat management.
 * 
 * @details
 * The service handles:
 * - Creating new bookings with seat selection
 * - Retrieving booking history for users
 * - Checking seat availability for showtimes
 * - Managing booking state transitions
 * 
 * @see BookingService
 * @see BookingView
 * @see SeatView
 * @since v1.0
 */
class IBookingService {
public:
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~IBookingService() = default;
    
    /**
     * @brief Creates a new booking for specified seats and showtime
     * 
     * Reserves selected seats for a user at a specific showtime,
     * performing validation and updating seat availability.
     * 
     * @param userID The ID of the user making the booking
     * @param showTimeID The ID of the movie showtime
     * @param seats Vector of seat identifiers to be booked
     * 
     * @pre userID must be a valid registered user
     * @pre showTimeID must be a valid future showtime
     * @pre All seats must be available for booking
     * @post Seats are reserved and marked as unavailable
     * @post Booking record is created in the system
     * 
     * @throws std::invalid_argument if userID or showTimeID is invalid
     * @throws std::runtime_error if seats are already booked
     * @throws std::runtime_error if booking creation fails
     * 
     * @note This operation should be atomic to prevent double-booking
     * @warning This method may block during database transactions
     * 
     * @see viewSeatsStatus()
     * @since v1.0
     */
    virtual void createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) = 0;
    
    /**
     * @brief Retrieves booking history for a specific user
     * 
     * Returns all bookings made by the specified user, including
     * past and upcoming reservations with detailed information.
     * 
     * @param userID The ID of the user whose history to retrieve
     * 
     * @return std::vector<BookingView> containing user's booking history
     *         sorted by booking date (most recent first)
     * 
     * @pre userID must be a valid registered user
     * @post Returns empty vector if user has no bookings
     * 
     * @throws std::invalid_argument if userID is invalid
     * 
     * @note Results are read-only views of booking data
     * 
     * @see BookingView
     * @since v1.0
     */
    virtual std::vector<BookingView> viewBookingHistory(const int& userID) = 0;
    
    /**
     * @brief Retrieves seat availability status for a showtime
     * 
     * Returns detailed information about all seats for a specific
     * showtime, including availability status and seat types.
     * 
     * @param showTimeID The ID of the showtime to check
     * 
     * @return std::vector<SeatView> containing seat status information
     *         organized by seat location
     * 
     * @pre showTimeID must be a valid showtime
     * @post Returns complete seat map with current availability
     * 
     * @throws std::invalid_argument if showTimeID is invalid
     * 
     * @note Seat status reflects real-time availability
     * @warning Status may change between calls due to concurrent bookings
     * 
     * @see SeatView
     * @see createBooking()
     * @since v1.0
     */
    virtual std::vector<SeatView> viewSeatsStatus(const int& showTimeID) = 0;
};

#endif