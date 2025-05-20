#ifndef IBOOKINGSERVICE_H
#define IBOOKINGSERVICE_H

class IBookingService {
public:
    virtual ~IBookingService() = default;
    virtual void bookTicket() = 0;
    virtual void cancelBooking() = 0;
    virtual void viewShowtimes() = 0;
    virtual void viewAvailableSeats() = 0;
};

#endif
