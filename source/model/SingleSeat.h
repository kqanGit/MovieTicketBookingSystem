/**
 * @file SingleSeat.h
 * @brief Single seat implementation for standard movie theater seating
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef _SINGLESEAT_H_
#define _SINGLESEAT_H_
#include "ISeat.h"
#include <string>

/**
 * @class SingleSeat
 * @brief Concrete implementation of ISeat for individual seating
 * 
 * This class represents a standard single seat in the movie theater, designed
 * for one person. Single seats are the most common seating type and typically
 * offer standard pricing and comfort levels.
 * 
 * @details
 * Key Features:
 * - Standard individual seating option
 * - Base pricing tier for theater seating
 * - Designed for single occupancy
 * - Standard comfort and space allocation
 * 
 * Design Patterns:
 * - **Strategy Pattern**: Implements ISeat interface with single-seat-specific behavior
 * - **Factory Pattern**: Created through SeatFactory for consistent instantiation
 * - **Value Object**: Immutable seat representation with defined properties
 * - **Template Method**: Follows ISeat contract for uniform seat handling
 * 
 * Seat Characteristics:
 * - Individual occupancy (one person)
 * - Standard pricing structure
 * - Basic comfort features
 * - Efficient theater space utilization
 * 
 * @par Usage Example
 * @code
 * // Create single seat through factory
 * auto singleSeat = std::make_unique<SingleSeat>("A15", SeatType::SINGLE, 12.99f);
 * 
 * std::cout << "Seat ID: " << singleSeat->id() << std::endl;
 * std::cout << "Price: $" << singleSeat->price() << std::endl;
 * std::cout << "Type: " << static_cast<int>(singleSeat->type()) << std::endl;
 * 
 * // Use in booking context
 * std::vector<std::shared_ptr<ISeat>> selectedSeats = {singleSeat};
 * bookingService->reserveSeats(showTimeId, selectedSeats);
 * @endcode
 * 
 * @note Single seats form the majority of theater seating inventory
 * @warning Ensure proper seat ID format and positive pricing
 * 
 * @see ISeat
 * @see SeatFactory
 * @see CoupleSeat
 * @see SeatType
 */
class SingleSeat : public ISeat {
private:
    /**
     * @brief Unique seat identifier
     * 
     * String identifier that uniquely identifies this seat within the theater.
     * Typically follows a format like "A15" (row A, seat 15).
     * 
     * @note Must be unique within the theater/showtime context
     */
    std::string _seatID;
    
    /**
     * @brief Seat price in currency units
     * 
     * The cost for booking this single seat. Single seats typically
     * have standard pricing that serves as the baseline for other seat types.
     * 
     * @pre Must be positive value
     * @note Price may vary based on showtime, movie popularity, seat location, etc.
     */
    float _price;
    
    /**
     * @brief Seat type classification
     * 
     * Indicates this is a single seat type for booking system
     * categorization and pricing logic.
     * 
     * @note Should always be SeatType::SINGLE for this class
     */
    SeatType _type;

public:
    /**
     * @brief Constructor for single seat
     * 
     * Creates a new single seat with specified identifier, type, and price.
     * Initializes all seat properties for use in the booking system.
     * 
     * @param seatID Unique identifier for the seat (e.g., "A15")
     * @param type Seat type (should be SeatType::SINGLE)
     * @param price Cost for booking this seat (must be positive)
     * 
     * @pre seatID is non-empty and unique within the theater
     * @pre price > 0.0f
     * @pre type == SeatType::SINGLE
     * @post Seat is ready for booking operations
     * 
     * @throws std::invalid_argument If seatID is empty or price is negative
     * 
     * @par Example
     * @code
     * SingleSeat seat("A15", SeatType::SINGLE, 12.99f);
     * @endcode
     * 
     * @note Constructor should validate input parameters for data integrity
     */
    SingleSeat(std::string seatID, SeatType type, float price);
    
    /**
     * @brief Get the seat price
     * 
     * Returns the cost for booking this single seat. Single seats
     * typically have standard pricing that serves as the baseline
     * for the theater's pricing structure.
     * 
     * @return float The price for this seat in currency units
     * 
     * @post Returns positive value
     * 
     * @note Price may be subject to dynamic pricing based on demand
     * @see ISeat::price()
     */
    float price() const override;
    
    /**
     * @brief Get the seat type
     * 
     * Returns the seat type classification, which should always
     * be SeatType::SINGLE for this implementation.
     * 
     * @return SeatType The seat type (SeatType::SINGLE)
     * 
     * @post Returns SeatType::SINGLE
     * 
     * @note Used for pricing, layout, and booking logic differentiation
     * @see ISeat::type()
     * @see SeatType
     */
    SeatType type() const override;
    
    /**
     * @brief Get the seat identifier
     * 
     * Returns the unique string identifier for this seat, used for
     * booking references and theater layout management.
     * 
     * @return std::string The unique seat identifier
     * 
     * @post Returns non-empty string
     * 
     * @note Seat ID format should be consistent across the theater
     * @see ISeat::id()
     */
    std::string id() const override;
};
#endif 