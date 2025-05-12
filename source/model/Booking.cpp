#include "Booking.h"
Booking::Booking(
    const int& bookingID,
    const int& movieID,
    const std::string& movieTitle,
    const ShowTime& showTime
) : bookingID(bookingID), movieID(movieID), movieTitle(movieTitle), showTime(showTime) {}