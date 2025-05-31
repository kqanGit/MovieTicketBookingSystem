/**
 * @file IVisitor.h
 * @brief Base interface for the Visitor pattern implementation
 * @author Movie Ticket Booking System Team
 */

#ifndef IVISITOR_H  
#define IVISITOR_H

/**
 * @interface IVisitor
 * @brief Base interface for all visitor implementations
 * 
 * This interface serves as the foundation for the Visitor pattern implementation
 * in the movie ticket booking system. It enables adding new operations to
 * existing class hierarchies without modifying the classes themselves.
 * 
 * @details
 * The Visitor pattern is used throughout the system to:
 * - Separate algorithms from the objects they operate on
 * - Add new operations without changing existing code
 * - Implement double dispatch for service operations
 * - Maintain clean separation of concerns
 * 
 * **Key Benefits:**
 * - **Open/Closed Principle**: Open for extension, closed for modification
 * - **Single Responsibility**: Each visitor handles one specific operation
 * - **Double Dispatch**: Method selection based on both visitor and visited object types
 * - **Maintainability**: Easy to add new operations without touching existing classes
 * 
 * **Usage Pattern:**
 * 1. Define specific visitor interfaces inheriting from IVisitor
 * 2. Implement concrete visitors for each operation
 * 3. Objects accept visitors and delegate operations
 * 4. Visitors perform operations based on object context
 * 
 * @see IServiceVisitor
 * @see LoginServiceVisitor
 * @see BookingServiceVisitor
 * @see IUserContext::accept()
 * @since v1.0
 */
class IVisitor {
public:
    /**
     * @brief Virtual destructor for proper inheritance
     * 
     * Ensures proper cleanup of visitor implementations when
     * accessed through base interface pointers.
     */
    virtual ~IVisitor() = default;
};

#endif