/**
 * @file BookingRepositorySQL.h
 * @brief SQLite implementation of booking repository interface
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef _BOOKINGREPOSITORY_H_
#define _BOOKINGREPOSITORY_H_
#include "IBookingRepository.h"
#include "../database/DatabaseConnection.h"
#include "../model/ShowTime.h"
#include "../model/Booking.h"
#include <stdexcept>
#include <map>
#include <format>

/**
 * @class BookingRepository
 * @brief SQLite-based implementation of booking data access operations
 * 
 * This class provides concrete implementation of booking repository
 * operations using SQLite database backend. It handles all booking-related
 * database operations including seat reservations, booking history, and
 * availability management.
 * 
 * @details
 * Key Features:
 * - Complete booking lifecycle management
 * - Seat availability tracking and reservation
 * - User booking history and retrieval
 * - Theater seat status management
 * - Transaction support for booking operations
 * 
 * Design Patterns:
 * - **Repository Pattern**: Concrete implementation of IBookingRepository
 * - **Data Access Object (DAO)**: Encapsulates booking database operations
 * - **Unit of Work**: Manages booking transactions and seat reservations
 * - **View Object**: Uses BookingView and SeatView for data presentation
 * 
 * Database Operations:
 * - Booking creation and management
 * - Seat reservation and status tracking
 * - User booking history queries
 * - Showtime seat availability checks
 * 
 * @par Usage Example
 * @code
 * auto bookingRepo = std::make_unique<BookingRepository>("database.db");
 * 
 * // Create a booking
 * bookingRepo->addBooking(userId, showTimeId);
 * int bookingId = bookingRepo->getLatestBookingID(userId);
 * 
 * // Reserve seats
 * std::vector<std::string> seats = {"A1", "A2"};
 * bookingRepo->addBookedSeats(bookingId, seats);
 * 
 * // View booking history
 * auto bookings = bookingRepo->viewAllBookings(userId);
 * @endcode
 * 
 * @note All operations are transactional to ensure data consistency
 * @warning Database connection must remain valid throughout repository lifetime
 * 
 * @see IBookingRepository
 * @see BookingView
 * @see SeatView
 * @see DatabaseConnection
 */
class BookingRepository : public IBookingRepository {
private:
    /**
     * @brief Database connection for booking operations
     * 
     * Pointer to the database connection used for all booking-related
     * database operations. Initialized during construction.
     * 
     * @note Repository owns and manages this connection
     */
    DatabaseConnection* _dbConnection;

public:
    /**
     * @brief Constructor with database file path
     * 
     * Initializes the booking repository with a database connection
     * to the specified SQLite database file.
     * 
     * @param dbFilePath Path to the SQLite database file
     * 
     * @pre Database file path is valid and accessible
     * @post Database connection is established
     * @post Repository is ready for booking operations
     * 
     * @throws std::runtime_error If database connection fails
     * 
     * @note Creates database file if it doesn't exist
     * @see DatabaseConnection
     */
    BookingRepository(std::string dbFilePath);
    
    /**
     * @brief Create a new booking for user and showtime
     * 
     * Creates a new booking record linking a user to a specific showtime.
     * This is the first step in the booking process before seat selection.
     * 
     * @param userID Unique identifier of the user making the booking
     * @param showTimeID Unique identifier of the movie showtime
     * 
     * @pre userID > 0 (valid user)
     * @pre showTimeID > 0 (valid showtime)
     * @post New booking record is created in database
     * @post Booking can be retrieved using getLatestBookingID()
     * 
     * @throws std::runtime_error If booking creation fails
     * 
     * @par Database Operations
     * - Validates user and showtime existence
     * - Creates booking record with timestamp
     * - Returns generated booking ID for subsequent operations
     * 
     * @note This method only creates the booking container, not seat reservations
     * @see addBookedSeats() for seat reservation
     * @see getLatestBookingID() to retrieve the created booking ID
     */
    void addBooking(const int& userID, const int& showTimeID) override;
    
    /**
     * @brief Add booked seats to an existing booking
     * 
     * Associates specific seats with a booking, completing the reservation process.
     * Updates seat availability and creates booking-seat relationships.
     * 
     * @param bookingID Unique identifier of the booking
     * @param bookedSeats Vector of seat IDs to be reserved
     * 
     * @pre bookingID > 0 (valid booking exists)
     * @pre bookedSeats is non-empty
     * @pre All seats in bookedSeats are available for the showtime
     * @post All specified seats are marked as booked
     * @post Booking-seat relationships are created in database
     * 
     * @throws std::runtime_error If any seat is already booked
     * @throws std::runtime_error If booking doesn't exist
     * 
     * @par Atomicity
     * - All seats are booked as a single transaction
     * - If any seat fails, entire operation is rolled back
     * - Ensures booking consistency
     * 
     * @note Seat IDs must correspond to valid seats for the showtime
     * @see addBooking() to create the booking first
     * @see viewSeatsStatus() to check seat availability
     */
    void addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) override;
    
    /**
     * @brief Get the most recent booking ID for a user
     * 
     * Retrieves the booking ID of the user's most recently created booking.
     * Used to link seat reservations to newly created bookings.
     * 
     * @param userID Unique identifier of the user
     * @return int The latest booking ID for the user
     * 
     * @retval Positive integer if user has bookings
     * @retval 0 or negative if no bookings found
     * 
     * @pre userID > 0 (valid user)
     * 
     * @note Bookings are ordered by creation timestamp
     * @see addBooking() creates bookings that can be retrieved
     */
    int getLatestBookingID(const int& userID) override;
    
    /**
     * @brief Retrieve all bookings for a specific user
     * 
     * Gets complete booking history for a user including movie details,
     * showtime information, reserved seats, and pricing.
     * 
     * @param userID Unique identifier of the user
     * @return std::vector<BookingView> Collection of user's booking information
     * 
     * @retval Empty vector if user has no bookings
     * @retval Vector of BookingView objects with complete booking details
     * 
     * @pre userID > 0 (valid user)
     * @post Each BookingView contains complete booking information
     * 
     * @par BookingView Contents
     * - Booking identification and metadata
     * - Movie information (title, ID)
     * - Showtime details (date, time)
     * - Reserved seat information
     * - Total booking price
     * 
     * @note Results are typically ordered by booking date (newest first)
     * @see BookingView for complete data structure
     */
    std::vector<BookingView> viewAllBookings(const int& userID) override;
    
    /**
     * @brief Get seat availability status for a showtime
     * 
     * Retrieves the current status (available/booked) of all seats
     * for a specific movie showtime. Used for seat selection interface.
     * 
     * @param showTimeID Unique identifier of the movie showtime
     * @return std::vector<SeatView> Collection of seat status information
     * 
     * @retval Vector of SeatView objects showing seat availability
     * @retval Empty vector if showtime doesn't exist
     * 
     * @pre showTimeID > 0 (valid showtime)
     * @post Each SeatView contains seat details and current status
     * 
     * @par SeatView Contents
     * - Seat information (ID, type, price)
     * - Current availability status
     * - Booking status for the specific showtime
     * 
     * @note Status reflects real-time seat availability
     * @see SeatView for complete data structure
     * @see addBookedSeats() for seat reservation
     */
    std::vector<SeatView> viewSeatsStatus(const int& showTimeID) override;
    
    /**
     * @brief Destructor for cleanup
     * 
     * Properly cleans up database connections and resources.
     * Ensures no memory leaks or connection issues.
     * 
     * @post All database connections are properly closed
     * @post All resources are released
     * 
     * @note Automatically called when repository is destroyed
     */
    ~BookingRepository();
};

#endif