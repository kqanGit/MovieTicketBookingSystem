#include <iostream>
#include "USER_pending/Interfaces.h"
#include "USER_pending/MovieManagerService.h"
#include "USER_pending/BookingService.h"
#include "USER_pending/ViewBookingHistoryService.h"

// A very simple test to ensure the services are initialized correctly
int main(int argc, char* argv[]) {
    std::cout << "Testing service initialization:" << std::endl;
    
    std::cout << "\nInitializing MovieManagerService:" << std::endl;
    MovieManagerService movieManager;
    
    std::cout << "\nTesting MovieManagerService functionality:" << std::endl;
    movieManager.addMovie();
    movieManager.updateMovie();
    movieManager.deleteMovie();
    movieManager.listMovies();
    
    std::cout << "\nInitializing BookingService:" << std::endl;
    BookingService bookingService;
    
    std::cout << "\nTesting BookingService functionality:" << std::endl;
    bookingService.bookTicket();
    bookingService.cancelBooking();
    bookingService.viewShowtimes();
    bookingService.viewAvailableSeats();
    
    std::cout << "\nInitializing ViewBookingHistoryService:" << std::endl;
    ViewBookingHistoryService historyService;
    
    std::cout << "\nTesting ViewBookingHistoryService functionality:" << std::endl;
    historyService.viewAllBookings();
    historyService.viewBookingDetails();
    
    return 0;
}
