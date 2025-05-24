#ifndef IVIEWBOOKINGHISTORYSERVICE_H
#define IVIEWBOOKINGHISTORYSERVICE_H

class IViewBookingHistoryService {
public:
    virtual ~IViewBookingHistoryService() = default;
    virtual void viewAllBookings() = 0;
    virtual void viewBookingDetails() = 0;
};


#endif
