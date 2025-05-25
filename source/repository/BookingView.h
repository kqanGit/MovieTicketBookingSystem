#ifndef _BOOKINGVIEW_H_
#define _BOOKINGVIEW_H_
#include "../model/ShowTime.h"
#include "../model/ISeat.h"
#include <vector>
#include <string>
#include <memory>
//DTO
class BookingView {
public:
    int bookingID;
    int movieID;
    std::string movieTitle;
    ShowTime showTime;
    std::vector<std::shared_ptr<ISeat>> bookedSeats;
    float totalPrice;
public:
    BookingView(const int& bookingID, 
                const int& movieID,
                const std::string& title,
                const ShowTime& showTime,
                const std::vector<std::shared_ptr<ISeat>>& bookedSeats,
                const float& totalPrice
    );
};

#endif
