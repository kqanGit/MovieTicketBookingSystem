/**
 * @file ShowTime.h
 * @brief Movie showtime data structure for scheduling information
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef _SHOWTIME_H_
#define _SHOWTIME_H_
#include <string>

/**
 * @struct ShowTime
 * @brief Data structure representing a movie showtime schedule
 * 
 * This structure encapsulates all temporal information about when a specific
 * movie is scheduled to be shown in the theater. It serves as a fundamental
 * data unit for the booking system's scheduling functionality.
 * 
 * @details
 * Key Features:
 * - Unique showtime identification
 * - Complete temporal scheduling information
 * - Date and time management for movie screenings
 * - Integration with booking and reservation systems
 * 
 * Design Patterns:
 * - **Value Object**: Immutable data container for showtime information
 * - **Data Transfer Object (DTO)**: Transfers showtime data between layers
 * - **Composite**: Combines multiple temporal attributes into single entity
 * 
 * Usage Context:
 * - Movie scheduling and timetable management
 * - Booking system reservation handling
 * - Theater capacity and availability tracking
 * - User interface showtime display
 * 
 * @par Usage Example
 * @code
 * ShowTime evening(101, "2025-06-15", "19:30", "22:00");
 * 
 * std::cout << "Showtime ID: " << evening.showTimeID << std::endl;
 * std::cout << "Date: " << evening.date << std::endl;
 * std::cout << "Duration: " << evening.startTime << " - " << evening.endTime << std::endl;
 * 
 * // Use in booking context
 * bookingService->reserveSeats(evening.showTimeID, selectedSeats);
 * @endcode
 * 
 * @note All time values should follow consistent format standards
 * @warning Ensure start time is before end time for data integrity
 * 
 * @see Movie
 * @see Booking
 * @see IBookingService
 * @see Theater
 */
struct ShowTime {
    /**
     * @brief Unique showtime identifier
     * 
     * Database primary key that uniquely identifies this specific showtime
     * across the entire system. Used for booking references and scheduling.
     * 
     * @note Must be unique across all showtimes in the database
     * @see Booking::showTimeID for foreign key relationships
     */
    int showTimeID;
    
    /**
     * @brief Date of the movie showing
     * 
     * The calendar date when this movie screening is scheduled.
     * Format should be consistent across the system (e.g., "YYYY-MM-DD").
     * 
     * @par Expected Format
     * - ISO 8601 date format: "YYYY-MM-DD"
     * - Example: "2025-06-15"
     * 
     * @note Date format should be validated for consistency
     * @see startTime, endTime for complete scheduling information
     */
    std::string date;
    
    /**
     * @brief Movie start time
     * 
     * The time when the movie screening begins on the scheduled date.
     * Should include sufficient lead time for previews and announcements.
     * 
     * @par Expected Format
     * - 24-hour format: "HH:MM"
     * - Example: "19:30" for 7:30 PM
     * 
     * @pre Must be earlier than endTime
     * @note Time format should be validated and consistent
     * @see endTime for screening duration calculation
     */
    std::string startTime;
    
    /**
     * @brief Movie end time
     * 
     * The time when the movie screening is expected to conclude.
     * Includes movie duration plus credits and buffer time.
     * 
     * @par Expected Format
     * - 24-hour format: "HH:MM"
     * - Example: "22:00" for 10:00 PM
     * 
     * @pre Must be later than startTime
     * @post Allows calculation of screening duration
     * 
     * @note Should account for full movie duration plus credits
     * @see startTime for complete time range
     */
    std::string endTime;

    /**
     * @brief Parameterized constructor
     * 
     * Creates a new showtime with all scheduling information specified.
     * Initializes all temporal attributes for the movie screening.
     * 
     * @param showTimeID Unique identifier for this showtime
     * @param date Date of screening (format: "YYYY-MM-DD")
     * @param startTime Movie start time (format: "HH:MM")
     * @param endTime Movie end time (format: "HH:MM")
     * 
     * @pre showTimeID > 0 for valid showtimes
     * @pre date is in valid format and not in the past
     * @pre startTime is in valid format and before endTime
     * @pre endTime is in valid format and after startTime
     * @post All showtime attributes are initialized
     * 
     * @throws std::invalid_argument If any parameter is invalid
     * 
     * @par Validation Considerations
     * - Verify date is not in the past
     * - Ensure start time precedes end time
     * - Validate time format consistency
     * - Check for scheduling conflicts
     * 
     * @par Example
     * @code
     * // Create evening showtime
     * ShowTime eveningShow(101, "2025-06-15", "19:30", "22:00");
     * 
     * // Create matinee showtime
     * ShowTime matineeShow(102, "2025-06-15", "14:00", "16:30");
     * @endcode
     * 
     * @note Constructor should validate input parameters for data integrity
     * @see Default constructor (if needed for STL containers)
     */
    ShowTime(int showTimeID, const std::string& date, const std::string& startTime, const std::string& endTime);
    
    /**
     * @brief Default constructor
     * 
     * Creates an uninitialized showtime object. Useful for STL containers
     * and scenarios where showtime data will be assigned later.
     * 
     * @post showTimeID == 0 (invalid state)
     * @post All string fields are empty
     * 
     * @note Requires subsequent initialization before use
     * @warning Object is in invalid state until properly initialized
     */
    ShowTime() : showTimeID(0) {}
};

#endif