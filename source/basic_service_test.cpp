#include <iostream>
#include <string>
#include "USER_pending/MovieManagerService.h"
#include "USER_pending/BookingService.h"
#include "USER_pending/ViewBookingHistoryService.h"

int main(int argc, char* argv[]) {
    std::cout << "Starting basic service test" << std::endl;
    
    // Test movie management functionality for Admin
    std::cout << "\n=== Testing Movie Management (Admin) ===" << std::endl;
    MovieManagerService movieManager;
    std::cout << "\nAdding a movie:" << std::endl;
    movieManager.addMovie();
    
    std::cout << "\nUpdating a movie:" << std::endl;
    movieManager.updateMovie();
    
    std::cout << "\nDeleting a movie:" << std::endl;
    movieManager.deleteMovie();
    
    std::cout << "\nListing all movies:" << std::endl;
    movieManager.listMovies();
    
    // Test booking functionality for User
    std::cout << "\n\n=== Testing Booking (User) ===" << std::endl;
    BookingService bookingService;
    
    std::cout << "\nBooking a ticket:" << std::endl;
    bookingService.bookTicket();
    
    std::cout << "\nCanceling a booking:" << std::endl;
    bookingService.cancelBooking();
    
    std::cout << "\nViewing showtimes:" << std::endl;
    bookingService.viewShowtimes();
    
    std::cout << "\nViewing available seats:" << std::endl;
    bookingService.viewAvailableSeats();
    
    // Test viewing booking history for User
    std::cout << "\n\n=== Testing Booking History (User) ===" << std::endl;
    ViewBookingHistoryService historyService;
    
    std::cout << "\nViewing all bookings:" << std::endl;
    historyService.viewAllBookings();
    
    std::cout << "\nViewing booking details:" << std::endl;
    historyService.viewBookingDetails();
    
    std::cout << "\nTest completed successfully!" << std::endl;
    
    return 0;
}
