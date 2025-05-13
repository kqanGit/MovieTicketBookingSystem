#ifndef _IBOOKINGSERVICE_H_
#define _IBOOKINGSERVICE_H_
#include <string>
#include <vector>

class IBookingService {
public:
    virtual ~IBookingService() = default;
    virtual void createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) = 0;
    virtual std::vector<std::string> viewSeatsStatus(const int& showTimeID) = 0;
};

#endif