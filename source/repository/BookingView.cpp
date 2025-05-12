#include "BookingView.h"

BookingView::BookingView(
    const int& bookingID, 
    const int& movieID,
    const std::string& title,
    const ShowTime& showTime,
    const std::vector<std::shared_ptr<ISeat>>& bookedSeats,
    const float& totalPrice
) : bookingID(bookingID), movieID(movieID), movieTitle(title), showTime(showTime), bookedSeats(bookedSeats), totalPrice(totalPrice) {}