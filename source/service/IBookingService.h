#ifndef _IBOOKINGSERVICE_H_
#define _IBOOKINGSERVICE_H_
#include <string>
#include <vector>
#include "SeatView.h"
#include "BookingView.h"

class IBookingService {
public:
    virtual ~IBookingService() = default;
    virtual void createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) = 0;
    virtual std::vector<BookingView> viewBookingHistory(const int& userID) = 0;
    virtual std::vector<SeatView> viewSeatsStatus(const int& showTimeID) = 0;
};

#endif