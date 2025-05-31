/**
 * @file IBookingRepository.h
 * @brief Booking repository interface definition
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef _IBOOKINGREPOSITORY_H_
#define _IBOOKINGREPOSITORY_H_
#include <string>
#include <vector>
#include "BookingView.h"
#include "SeatView.h"

/**
 * @interface IBookingRepository
 * @brief Interface for booking data access operations
 * 
 * The IBookingRepository interface defines the contract for all booking-related
 * data access operations in the movie ticket booking system. This interface
 * follows the Repository pattern, providing an abstraction layer between
 * the business logic and data persistence mechanisms.
 * 
 * @details Key Responsibilities:
 * - Manage booking creation and persistence
 * - Handle seat reservations and associations
 * - Provide booking history and status information
 * - Abstract database-specific operations
 * 
 * Design Pattern: Repository Pattern
 * - Encapsulates data access logic
 * - Provides testability through interface abstraction
 * - Enables multiple storage implementations (SQL, NoSQL, etc.)
 * - Centralizes booking-related data operations
 * 
 * Implementation Considerations:
 * - All operations should be transactional where appropriate
 * - Thread safety should be handled by concrete implementations
 * - Error handling should use exceptions for exceptional cases
 * - Data validation should occur at the service layer
 * 
 * Usage Example:
 * @code
 * std::unique_ptr<IBookingRepository> bookingRepo = 
 *     std::make_unique<BookingRepositorySQL>(dbConnection);
 * 
 * // Create a new booking
 * bookingRepo->addBooking(userId, showTimeId);
 * int bookingId = bookingRepo->getLatestBookingID(userId);
 * 
 * // Reserve seats for the booking
 * std::vector<std::string> seats = {"A1", "A2"};
 * bookingRepo->addBookedSeats(bookingId, seats);
 * 
 * // View booking history
 * auto bookings = bookingRepo->viewAllBookings(userId);
 * @endcode
 * 
 * @see BookingRepositorySQL
 * @see BookingView
 * @see SeatView
 * @since 1.0
 */
class IBookingRepository {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     * 
     * Ensures that concrete repository implementations are properly
     * destroyed when accessed through the interface pointer.
     */
    virtual ~IBookingRepository() = default;
    
    /**
     * @brief Creates a new booking record for a user and showtime
     * 
     * Initiates a new booking transaction by creating a booking record
     * that associates a user with a specific movie showtime. This method
     * creates the primary booking entity but does not reserve specific seats.
     * 
     * @param userID Unique identifier of the user making the booking
     * @param showTimeID Unique identifier of the movie showtime being booked
     * 
     * @pre userID must be a valid, existing user identifier
     * @pre showTimeID must be a valid, existing showtime identifier
     * @pre The showtime must not be in the past
     * @pre The showtime must have available capacity
     * @post A new booking record is created in the system
     * @post The booking is associated with the specified user and showtime
     * 
     * @throw std::invalid_argument if userID or showTimeID is invalid
     * @throw std::runtime_error if booking creation fails due to system issues
     * @throw BookingException if showtime is full or no longer available
     * 
     * @note This method should be called before addBookedSeats()
     * @note Consider using database transactions for atomicity
     * 
     * Usage:
     * @code
     * try {
     *     repository->addBooking(12345, 67890);
     *     // Booking successfully created
     * } catch (const std::exception& e) {
     *     // Handle booking creation failure
     * }
     * @endcode
     */
    virtual void addBooking(const int& userID, const int& showTimeID) = 0;
    
    /**
     * @brief Associates specific seats with an existing booking
     * 
     * Reserves the specified seats for the given booking, completing the
     * seat selection process. This method should be called after a booking
     * has been created with addBooking().
     * 
     * @param bookingID Unique identifier of the booking to associate seats with
     * @param bookedSeats Vector of seat identifiers to reserve (e.g., "A1", "B5")
     * 
     * @pre bookingID must be a valid, existing booking identifier
     * @pre bookedSeats must not be empty
     * @pre All seats in bookedSeats must be valid seat identifiers
     * @pre All seats must be available (not already booked)
     * @pre All seats must belong to the same showtime as the booking
     * @post All specified seats are marked as booked
     * @post Seats are associated with the booking
     * @post Other users cannot book these seats
     * 
     * @throw std::invalid_argument if bookingID is invalid or bookedSeats is empty
     * @throw SeatUnavailableException if any seat is already booked
     * @throw std::runtime_error if seat reservation fails due to system issues
     * 
     * @note This operation should be atomic - either all seats are booked or none
     * @note Consider implementing timeout mechanisms for long-running transactions
     * 
     * Usage:
     * @code
     * std::vector<std::string> selectedSeats = {"A1", "A2", "A3"};
     * try {
     *     repository->addBookedSeats(bookingId, selectedSeats);
     *     // All seats successfully reserved
     * } catch (const SeatUnavailableException& e) {
     *     // Some seats were already taken
     * }
     * @endcode
     */
    virtual void addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) = 0;
    
    /**
     * @brief Retrieves the most recent booking ID for a specific user
     * 
     * Returns the identifier of the most recently created booking for the
     * specified user. This is typically used immediately after creating
     * a booking to obtain its ID for subsequent operations.
     * 
     * @param userID Unique identifier of the user to query
     * @return The booking ID of the user's most recent booking
     * 
     * @pre userID must be a valid, existing user identifier
     * @pre The user must have at least one booking
     * @post Returns the most recent booking ID
     * 
     * @throw std::invalid_argument if userID is invalid
     * @throw NoBookingFoundException if user has no bookings
     * @throw std::runtime_error if query fails due to system issues
     * 
     * @note The "most recent" is determined by booking creation timestamp
     * @note This method is typically called immediately after addBooking()
     * 
     * Usage:
     * @code
     * repository->addBooking(userId, showTimeId);
     * int newBookingId = repository->getLatestBookingID(userId);
     * // Use newBookingId for seat reservation
     * @endcode
     */
    virtual int getLatestBookingID(const int& userID) = 0;
    
    /**
     * @brief Retrieves all booking history for a specific user
     * 
     * Returns a comprehensive list of all bookings made by the specified user,
     * including booking details, movie information, showtime data, and
     * associated seats. This provides a complete booking history view.
     * 
     * @param userID Unique identifier of the user to query
     * @return Vector of BookingView objects containing complete booking information
     * 
     * @pre userID must be a valid, existing user identifier
     * @post Returns all bookings for the user (may be empty if no bookings exist)
     * @post BookingView objects contain complete booking details
     * 
     * @throw std::invalid_argument if userID is invalid
     * @throw std::runtime_error if query fails due to system issues
     * 
     * @note Returns bookings in reverse chronological order (newest first)
     * @note Each BookingView contains denormalized data for efficient display
     * @note Empty vector is returned if user has no bookings
     * 
     * Usage:
     * @code
     * auto userBookings = repository->viewAllBookings(userId);
     * for (const auto& booking : userBookings) {
     *     std::cout << "Movie: " << booking.movieTitle 
     *               << " Date: " << booking.showTime << std::endl;
     * }
     * @endcode
     */
    virtual std::vector<BookingView> viewAllBookings(const int& userID) = 0;
    
    /**
     * @brief Retrieves seat availability status for a specific showtime
     * 
     * Returns the current booking status of all seats for the specified
     * showtime, enabling seat selection interfaces to display available
     * and booked seats appropriately.
     * 
     * @param showTimeID Unique identifier of the showtime to query
     * @return Vector of SeatView objects containing seat details and status
     * 
     * @pre showTimeID must be a valid, existing showtime identifier
     * @post Returns all seats for the showtime with current status
     * @post SeatView objects contain seat details and availability status
     * 
     * @throw std::invalid_argument if showTimeID is invalid
     * @throw std::runtime_error if query fails due to system issues
     * 
     * @note Status reflects real-time availability
     * @note Includes both available and booked seats
     * @note Seat order may be implementation-dependent
     * 
     * Usage:
     * @code
     * auto seatStatus = repository->viewSeatsStatus(showTimeId);
     * for (const auto& seatView : seatStatus) {
     *     if (seatView.status == SeatStatus::AVAILABLE) {
     *         renderAvailableSeat(seatView.seat);
     *     } else {
     *         renderBookedSeat(seatView.seat);
     *     }
     * }
     * @endcode
     */
    virtual std::vector<SeatView> viewSeatsStatus(const int& showTimeID) = 0;
};

#endif