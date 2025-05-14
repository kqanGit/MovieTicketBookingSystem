#include "BookingService.h"

BookingService::BookingService() {
    repo = new BookingRepository("database.db");
}

void BookingService::createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) {
    repo->addBooking(userID, showTimeID);
    int bookingID = repo->getLatestBookingID(userID);
    repo->addBookedSeats(bookingID, seats);
}

std::vector<SeatView> BookingService::viewSeatsStatus(const int& showTimeID) {
    return repo->viewSeatsStatus(showTimeID);
}

std::vector<BookingView> BookingService::viewBookingHistory(const int& userID) {
    return repo->viewAllBookings(userID);
}