#ifndef _BOOKINGSERVICE_H_
#define _BOOKINGSERVICE_H_
#include "IBookingService.h"
#include "IBookingRepository.h"
#include "BookingRepositorySQL.h"
#include <memory>

class BookingService : public IBookingService {
private:
    std::shared_ptr<IBookingRepository> _repo;
public:
    BookingService(std::shared_ptr<IBookingRepository> repo);
    void createBooking(const int& userID, const int& showTimeID, const std::vector<std::string>& seats) override;
    std::vector<BookingView> viewBookingHistory(const int& userID) override;
    std::vector<SeatView> viewSeatsStatus(const int& showTimeID) override;
};

#endif