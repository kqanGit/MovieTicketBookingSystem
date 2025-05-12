#ifndef _BOOKING_H_
#define _BOOKING_H_
#include <string>
#include "ShowTime.h"
struct Booking {
    int bookingID;
    int movieID;
    std::string movieTitle;
    ShowTime showTime;
    Booking(
        const int& bookingID,
        const int& movieID,
        const std::string& movieTitle,
        const ShowTime& showTime
    );
};
#endif  