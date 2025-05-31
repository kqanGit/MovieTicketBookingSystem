/**
 * @file SeatView.h
 * @brief Seat view model for UI representation
 * @author Movie Ticket Booking System Team
 * @date 2024
 */

#ifndef _SEATVIEW_H_
#define _SEATVIEW_H_
#include "../model/ISeat.h"
#include <memory>

/**
 * @enum SeatStatus
 * @brief Enumeration representing the availability status of a seat
 * 
 * This enumeration defines the possible states a seat can be in
 * within the booking system context.
 */
enum SeatStatus {
    AVAILABLE, ///< Seat is available for booking
    BOOKED,    ///< Seat has been reserved/booked
};

/**
 * @class SeatView
 * @brief View model combining seat information with its booking status
 * 
 * The SeatView class implements the View Model pattern, providing a composite
 * object that combines seat domain data with presentation-specific status
 * information. This class is primarily used by the UI layer to display
 * seat information along with their current booking status.
 * 
 * @details Key Features:
 * - Combines domain object (ISeat) with status information
 * - Lightweight wrapper for UI presentation
 * - Immutable design after construction
 * - Thread-safe for read operations
 * 
 * Design Pattern: View Model
 * - Separates presentation logic from domain logic
 * - Provides UI-specific data structure
 * - Enables clean data binding in user interfaces
 * 
 * Usage Example:
 * @code
 * // Create a seat view for UI display
 * std::shared_ptr<ISeat> seat = seatFactory.createSingleSeat("A1", 100.0);
 * SeatView seatView(seat, SeatStatus::AVAILABLE);
 * 
 * // Use in UI rendering
 * if (seatView.status == SeatStatus::AVAILABLE) {
 *     renderAvailableSeat(seatView.seat);
 * } else {
 *     renderBookedSeat(seatView.seat);
 * }
 * @endcode
 * 
 * @see ISeat
 * @see SeatStatus
 * @since 1.0
 */
class SeatView {
public:
    /**
     * @brief The seat domain object containing seat details
     * 
     * Shared pointer to the underlying seat entity that contains
     * all the seat-specific information such as ID, type, and price.
     * 
     * @note This should never be null after construction
     */
    std::shared_ptr<ISeat> seat;
    
    /**
     * @brief Current booking status of the seat
     * 
     * Indicates whether the seat is currently available for booking
     * or has already been reserved by another customer.
     */
    SeatStatus status;

public:
    /**
     * @brief Constructs a SeatView with seat data and status
     * 
     * Creates a new view model instance that combines the seat domain
     * object with its current booking status for UI presentation.
     * 
     * @param seat Shared pointer to the seat entity
     * @param status Current booking status of the seat
     * 
     * @pre seat must not be null
     * @pre status must be a valid SeatStatus value
     * @post SeatView is initialized with provided seat and status
     * 
     * @throw std::invalid_argument if seat is null
     * 
     * @note The constructor creates a lightweight wrapper without copying seat data
     * @note The seat object is shared, so modifications to the underlying seat
     *       will be reflected in this view
     * 
     * Usage:
     * @code
     * auto seat = std::make_shared<SingleSeat>("B5", 150.0);
     * SeatView view(seat, SeatStatus::AVAILABLE);
     * 
     * // View is ready for UI binding
     * displaySeat(view);
     * @endcode
     */
    SeatView(std::shared_ptr<ISeat> seat, const SeatStatus& status);
};

#endif 