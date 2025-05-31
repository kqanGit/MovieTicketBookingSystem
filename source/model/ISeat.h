/**
 * @file ISeat.h
 * @brief Interface and types for theater seat management
 * @author Movie Ticket Booking System Team
 */

#ifndef _ISEAT_H_
#define _ISEAT_H_
#include <string>

/**
 * @enum SeatType
 * @brief Enumeration of available seat types in the theater
 * 
 * Defines the different categories of seats available for booking,
 * each with potentially different pricing and capacity.
 * 
 * @since v1.0
 */
enum SeatType{
    SINGLE, ///< Standard single-person seat
    COUPLE  ///< Double-width seat for couples
};

/**
 * @interface ISeat
 * @brief Abstract interface for theater seat implementations
 * 
 * This interface defines the contract for all seat types in the theater,
 * implementing the Strategy pattern to handle different seat behaviors
 * and pricing models.
 * 
 * @details
 * Design patterns implemented:
 * - **Strategy Pattern**: Different seat types with varying behaviors
 * - **Template Method**: Common interface with specific implementations
 * - **Factory Pattern**: Used with SeatFactory for seat creation
 * 
 * Key responsibilities:
 * - Define pricing strategy for seat type
 * - Provide unique identification for seats
 * - Classify seat type for booking logic
 * - Support seat reservation and management
 * 
 * @see SingleSeat
 * @see CoupleSeat
 * @see SeatFactory
 * @since v1.0
 */
class ISeat {
public:
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~ISeat() = default;
    
    /**
     * @brief Calculates the price for this seat
     * 
     * Returns the current price for booking this seat, which may
     * vary based on seat type, location, showtime, or other factors.
     * 
     * @return float The price of the seat in system currency
     * 
     * @post Returns positive price value
     * 
     * @note Price calculation may include dynamic pricing factors
     * @note Currency and precision are system-dependent
     * 
     * @since v1.0
     */
    virtual float price() const = 0;
    
    /**
     * @brief Gets the unique identifier for this seat
     * 
     * Returns a unique string identifier that can be used to
     * reference this specific seat in bookings and reservations.
     * 
     * @return std::string Unique seat identifier (e.g., "A1", "B2")
     * 
     * @post Returns non-empty unique identifier
     * 
     * @note ID format may follow theater-specific conventions
     * @note IDs should be human-readable for customer reference
     * 
     * @since v1.0
     */
    virtual std::string id() const = 0;
    
    /**
     * @brief Gets the type category of this seat
     * 
     * Returns the seat type classification for booking logic,
     * pricing, and display purposes.
     * 
     * @return SeatType The category of this seat
     * 
     * @post Returns valid SeatType enum value
     * 
     * @note Used for filtering and categorizing seats
     * @note Affects pricing and booking constraints
     * 
     * @see SeatType
     * @since v1.0
     */
    virtual SeatType type() const = 0;
};

#endif 