#ifndef VIEW_BOOKING_HISTORY_SERVICE_H
#define VIEW_BOOKING_HISTORY_SERVICE_H

// This is a simple implementation of the IViewBookingHistoryService interface
// To test calling the viewAllBookings and viewBookingDetails methods

#include "IViewBookingHistoryService.h"
#include <iostream>

class ViewBookingHistoryService : public IViewBookingHistoryService {
public:
    ViewBookingHistoryService() {
        std::cout << "ViewBookingHistoryService initialized" << std::endl;
    }
    
    void viewAllBookings() {
        std::cout << "User functionality: Viewing all bookings history" << std::endl;
        std::cout << "Booking ID: B001 | Movie: Spider-Man: No Way Home | Date: 2023-05-15 | Time: 19:30 | Seats: A1, A2" << std::endl;
        std::cout << "Booking ID: B002 | Movie: Dune | Date: 2023-05-20 | Time: 18:00 | Seats: C4, C5, C6" << std::endl;
    }
    
    void viewBookingDetails() {
        std::cout << "User functionality: Viewing detailed booking information" << std::endl;
        std::cout << "Booking Details for B001:" << std::endl;
        std::cout << "Movie: Spider-Man: No Way Home" << std::endl;
        std::cout << "Date: 2023-05-15" << std::endl;
        std::cout << "Time: 19:30" << std::endl;
        std::cout << "Cinema: Cinema 1" << std::endl;
        std::cout << "Seats: A1, A2" << std::endl;
        std::cout << "Total Amount: $20.00" << std::endl;
        std::cout << "Payment Status: Paid" << std::endl;
        std::cout << "Booking Date: 2023-05-10" << std::endl;
    }
};

#endif // VIEW_BOOKING_HISTORY_SERVICE_H