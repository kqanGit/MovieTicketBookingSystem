#ifndef _BOOKINGSERVICE_H_
#define _BOOKINGSERVICE_H_
#include "IBookingService.h"
#include "IBookingRepository.h"
#include "BookingRepositorySQL.h"

class BookingService : public IBookingService {
private:
    IBookingRepository* repo;
public:
    BookingService();
    void createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) override;
    std::vector<BookingView> viewBookingHistory(const int& userID) override;
    std::vector<SeatView> viewSeatsStatus(const int& showTimeID) override;
};

#endif