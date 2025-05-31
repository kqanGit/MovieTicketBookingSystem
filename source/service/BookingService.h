/**
 * @file BookingService.h
 * @brief Movie ticket booking service implementation
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef _BOOKINGSERVICE_H_
#define _BOOKINGSERVICE_H_

#include "IBookingService.h"
#include "../repository/IBookingRepository.h"
#include "../repository/BookingRepositorySQL.h"
#include <memory>

/**
 * @class BookingService
 * @brief Concrete implementation of movie ticket booking operations
 * 
 * This class handles all movie ticket booking-related business logic including
 * ticket reservation, booking history management, and seat availability checking.
 * It implements the IBookingService interface and coordinates with booking
 * repositories for data persistence.
 * 
 * @details
 * Key Features:
 * - Movie ticket reservation and booking
 * - User booking history retrieval
 * - Real-time seat availability checking
 * - Multi-seat booking support
 * - Integration with showtime management
 * - ACID transaction support for booking operations
 * 
 * @par Design Patterns Used
 * - Strategy Pattern: Implements IBookingService interface
 * - Repository Pattern: Uses IBookingRepository for data access
 * - Dependency Injection: Repository injected via constructor
 * - RAII: Uses smart pointers for automatic memory management
 * 
 * @par Usage Example
 * @code
 * auto bookingRepo = std::make_shared<BookingRepositorySQL>();
 * BookingService service(bookingRepo);
 * 
 * // Create a booking
 * std::vector<std::string> seats = {"A1", "A2"};
 * service.createBooking(userId, showtimeId, seats);
 * 
 * // View booking history
 * auto history = service.viewBookingHistory(userId);
 * @endcode
 * 
 * @warning All booking operations should be executed within database transactions
 * 
 * @see IBookingService
 * @see IBookingRepository
 * @see BookingView
 * @see SeatView
 */
class BookingService : public IBookingService {
private:
    /**
     * @brief Shared pointer to booking repository
     * 
     * Repository instance responsible for booking data persistence
     * and retrieval. Uses shared_ptr for safe memory management
     * and potential repository sharing across services.
     */
    std::shared_ptr<IBookingRepository> _repo;

public:
    /**
     * @brief Constructor with repository dependency injection
     * 
     * Initializes the booking service with a specific repository
     * implementation for data access operations.
     * 
     * @param repo Shared pointer to booking repository implementation
     * 
     * @pre repo != nullptr
     * @post _repo == repo
     * 
     * @throws std::invalid_argument if repo is nullptr
     * 
     * @par Example
     * @code
     * auto repo = std::make_shared<BookingRepositorySQL>();
     * BookingService service(repo);
     * @endcode
     */
    BookingService(std::shared_ptr<IBookingRepository> repo);

    /**
     * @brief Create a new movie ticket booking
     * 
     * Creates a booking for specified seats at a given showtime for a user.
     * Handles seat availability validation, booking persistence, and
     * seat status updates atomically.
     * 
     * @param userID Unique identifier of the user making the booking
     * @param showTimeID Unique identifier of the movie showtime
     * @param seats Vector of seat identifiers to reserve (e.g., "A1", "B5")
     * 
     * @pre userID > 0 (valid user identifier)
     * @pre showTimeID > 0 (valid showtime identifier)
     * @pre !seats.empty() (at least one seat to book)
     * @pre All seats in the vector are available for booking
     * 
     * @post Booking record created in database
     * @post All specified seats marked as occupied
     * @post User's booking history updated
     * 
     * @throws std::invalid_argument if any seat is already booked
     * @throws std::runtime_error if database operation fails
     * 
     * @par Transaction Behavior
     * This operation is atomic - either all seats are booked successfully
     * or none are booked if any error occurs.
     * 
     * @warning Concurrent booking attempts for same seats may conflict
     * 
     * @see viewSeatsStatus() to check availability before booking
     */
    void createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) override;

    /**
     * @brief Retrieve user's booking history
     * 
     * Fetches all bookings made by a specific user, including
     * past and upcoming movie showings with booking details.
     * 
     * @param userID Unique identifier of the user
     * @return std::vector<BookingView> List of user's booking records
     * 
     * @retval Empty vector if user has no bookings
     * @retval Vector of BookingView objects with booking details
     * 
     * @pre userID > 0 (valid user identifier)
     * 
     * @post Return value contains chronologically ordered booking history
     * 
     * @par Returned Information
     * Each BookingView includes:
     * - Booking ID and timestamp
     * - Movie title and showtime information
     * - Seat details and pricing
     * - Booking status (confirmed, cancelled, etc.)
     * 
     * @see BookingView for detailed structure information
     */
    std::vector<BookingView> viewBookingHistory(const int& userID) override;

    /**
     * @brief Check seat availability for a showtime
     * 
     * Retrieves the current availability status of all seats
     * for a specific movie showtime, enabling users to see
     * which seats are available for booking.
     * 
     * @param showTimeID Unique identifier of the movie showtime
     * @return std::vector<SeatView> List of all seats with their availability status
     * 
     * @retval Vector of SeatView objects representing theater layout
     * @retval Empty vector if showtime doesn't exist
     * 
     * @pre showTimeID > 0 (valid showtime identifier)
     * 
     * @post Return value reflects real-time seat availability
     * 
     * @par Seat Information Provided
     * Each SeatView includes:
     * - Seat identifier (row and number)
     * - Availability status (available, occupied, reserved)
     * - Seat type (regular, premium, couple)
     * - Pricing information
     * 
     * @note Results may change between calls due to concurrent bookings
     * 
     * @see SeatView for detailed seat information structure
     * @see createBooking() to reserve available seats
     */
    std::vector<SeatView> viewSeatsStatus(const int& showTimeID) override;
};

#endif