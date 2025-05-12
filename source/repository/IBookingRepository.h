#ifndef _IBOOKINGREPOSITORY_H_
#define _IBOOKINGREPOSITORY_H_
#include <string>
#include <vector>
#include "BookingView.h"

class IBookingRepository {
public:
    virtual ~IBookingRepository() = default;
    virtual void addBooking(const int& userID, const int& showTimeID) = 0;
    virtual void addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) = 0;
    virtual std::vector<BookingView> viewAllBookings(const int& userID) = 0;
};

#endif