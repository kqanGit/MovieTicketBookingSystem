/**
 * @file CoupleSeat.h
 * @brief Couple seat implementation for movie theater seating
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef _COUPLESEAT_H_
#define _COUPLESEAT_H_
#include "ISeat.h"
#include <string>

/**
 * @class CoupleSeat
 * @brief Concrete implementation of ISeat for couple seating
 * 
 * This class represents a couple seat in the movie theater, designed for
 * two people sitting together. Couple seats typically offer more space,
 * comfort, and privacy compared to regular single seats.
 * 
 * @details
 * Key Features:
 * - Premium seating option for couples
 * - Higher pricing tier than single seats
 * - Enhanced comfort and space
 * - Special seating arrangement for two people
 * 
 * Design Patterns:
 * - **Strategy Pattern**: Implements ISeat interface with couple-specific behavior
 * - **Factory Pattern**: Created through SeatFactory for consistent instantiation
 * - **Value Object**: Immutable seat representation with defined properties
 * 
 * Seat Characteristics:
 * - Designed for two occupants
 * - Premium pricing structure
 * - Enhanced comfort features
 * - Strategic theater placement
 * 
 * @par Usage Example
 * @code
 * // Create couple seat through factory
 * auto coupleSeat = std::make_unique<CoupleSeat>("C12", SeatType::COUPLE, 25.99f);
 * 
 * std::cout << "Seat ID: " << coupleSeat->id() << std::endl;
 * std::cout << "Price: $" << coupleSeat->price() << std::endl;
 * std::cout << "Type: " << static_cast<int>(coupleSeat->type()) << std::endl;
 * @endcode
 * 
 * @note Couple seats are typically more expensive than single seats
 * @warning Ensure proper seat ID format and positive pricing
 * 
 * @see ISeat
 * @see SeatFactory
 * @see SingleSeat
 * @see SeatType
 */
class CoupleSeat : public ISeat {
private:
    /**
     * @brief Unique seat identifier
     * 
     * String identifier that uniquely identifies this seat within the theater.
     * Typically follows a format like "C12" (row C, seat 12).
     * 
     * @note Must be unique within the theater/showtime context
     */
    std::string _seatID;
    
    /**
     * @brief Seat price in currency units
     * 
     * The cost for booking this couple seat. Couple seats typically
     * have premium pricing compared to single seats.
     * 
     * @pre Must be positive value
     * @note Price may vary based on showtime, movie popularity, etc.
     */
    float _price;
    
    /**
     * @brief Seat type classification
     * 
     * Indicates this is a couple seat type for booking system
     * categorization and special handling.
     * 
     * @note Should always be SeatType::COUPLE for this class
     */
    SeatType _type;

public:
    /**
     * @brief Constructor for couple seat
     * 
     * Creates a new couple seat with specified identifier, type, and price.
     * Initializes all seat properties for use in the booking system.
     * 
     * @param seatID Unique identifier for the seat (e.g., "C12")
     * @param type Seat type (should be SeatType::COUPLE)
     * @param price Cost for booking this seat (must be positive)
     * 
     * @pre seatID is non-empty and unique
     * @pre price > 0.0f
     * @pre type == SeatType::COUPLE
     * @post Seat is ready for booking operations
     * 
     * @throws std::invalid_argument If seatID is empty or price is negative
     * 
     * @par Example
     * @code
     * CoupleSeat seat("C12", SeatType::COUPLE, 25.99f);
     * @endcode
     */
    CoupleSeat(std::string seatID, SeatType type, float price);
    
    /**
     * @brief Get the seat price
     * 
     * Returns the cost for booking this couple seat. Couple seats
     * typically have premium pricing due to enhanced comfort and space.
     * 
     * @return float The price for this seat in currency units
     * 
     * @post Returns positive value
     * 
     * @note Price may be subject to dynamic pricing algorithms
     * @see ISeat::price()
     */
    float price() const override;
    
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
    
    /**
     * @brief Get the seat type
     * 
     * Returns the seat type classification, which should always
     * be SeatType::COUPLE for this implementation.
     * 
     * @return SeatType The seat type (SeatType::COUPLE)
     * 
     * @post Returns SeatType::COUPLE
     * 
     * @note Used for pricing, layout, and booking logic differentiation
     * @see ISeat::type()
     * @see SeatType
     */
    SeatType type() const override;
};
#endif