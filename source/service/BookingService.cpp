#include "BookingService.h"

BookingService::BookingService(IBookingRepository* repo) {
    _repo = repo;
}

void BookingService::createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) {
    _repo->addBooking(userID, showTimeID);
    int bookingID = _repo->getLatestBookingID(userID);
    _repo->addBookedSeats(bookingID, seats);
}

std::vector<SeatView> BookingService::viewSeatsStatus(const int& showTimeID) {
    return _repo->viewSeatsStatus(showTimeID);
}

std::vector<BookingView> BookingService::viewBookingHistory(const int& userID) {
    return _repo->viewAllBookings(userID);
}

BookingService::~BookingService() {
    delete _repo;
}