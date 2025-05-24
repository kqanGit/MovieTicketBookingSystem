#ifndef BOOKING_SERVICE_H
#define BOOKING_SERVICE_H

#include "IBookingService.h"
#include <iostream>

class BookingService : public IBookingService {
public:
    BookingService() {
        std::cout << "BookingService initialized" << std::endl;
    }
    
    void bookTicket() {
        std::cout << "User functionality: Booking a ticket" << std::endl;
    }
    
    void cancelBooking() {
        std::cout << "User functionality: Canceling a booking" << std::endl;
    }
    
    void viewShowtimes() {
        std::cout << "User functionality: Viewing showtimes" << std::endl;
    }
    
    void viewAvailableSeats() {
        std::cout << "User functionality: Viewing available seats" << std::endl;
    }
};

#endif // BOOKING_SERVICE_H