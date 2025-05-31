/**
 * @file SeatFactory.h
 * @brief Factory class for creating seat objects
 * @author Movie Ticket Booking System Team
 */

#ifndef _SEATFACTORY_H_
#define _SEATFACTORY_H_
#include "ISeat.h"
#include "SingleSeat.h"
#include "CoupleSeat.h"
#include <string>

/**
 * @class SeatFactory
 * @brief Factory for creating different types of seat objects
 * 
 * Implements the Factory Method pattern to create seat instances based on
 * seat type specification. This centralizes seat creation logic and ensures
 * consistent initialization of seat objects.
 * 
 * @details
 * Design patterns implemented:
 * - **Factory Method Pattern**: Creates objects without specifying exact classes
 * - **Simple Factory**: Provides a single method to create different seat types
 * 
 * Benefits:
 * - Encapsulates seat creation logic
 * - Easy to extend with new seat types
 * - Ensures proper initialization of seat objects
 * - Provides type safety for seat creation
 * 
 * @see ISeat
 * @see SingleSeat
 * @see CoupleSeat
 * @see SeatType
 * @since v1.0
 */
class SeatFactory {
private:

public:
    /**
     * @brief Creates a seat object based on type specification
     * 
     * Factory method that instantiates the appropriate seat subclass
     * based on the provided seat type and initializes it with the
     * given parameters.
     * 
     * @param seatID Unique identifier for the seat (e.g., "A1", "B2")
     * @param type The type of seat to create (SINGLE or COUPLE)
     * @param price The base price for this seat
     * 
     * @return ISeat* Pointer to the created seat object, or nullptr if type is invalid
     * 
     * @pre seatID must be a non-empty unique identifier
     * @pre type must be a valid SeatType enum value
     * @pre price must be non-negative
     * @post Returns fully initialized seat object if successful
     * @post Returns nullptr if seat type is not recognized
     * 
     * @note Caller is responsible for managing the returned pointer's lifetime
     * @warning Memory management: caller must delete the returned object
     * 
     * @throws std::invalid_argument if seatID is empty or price is negative
     * @throws std::bad_alloc if memory allocation fails
     * 
     * @code
     * SeatFactory factory;
     * ISeat* singleSeat = factory.createSeat("A1", SINGLE, 10.0f);
     * ISeat* coupleSeat = factory.createSeat("B1", COUPLE, 18.0f);
     * 
     * // Use seats...
     * 
     * delete singleSeat;
     * delete coupleSeat;
     * @endcode
     * 
     * @see ISeat
     * @see SeatType
     * @see SingleSeat
     * @see CoupleSeat
     * @since v1.0
     */
    ISeat* createSeat(std::string seatID, SeatType type, float price) {
        if (type == SINGLE) {
            return new SingleSeat(seatID, type, price);
        }
        else if (type == COUPLE) {
            return new CoupleSeat(seatID, type, price);
        }
        return nullptr; // Return nullptr if seat type is not found
    }
};
#endif