#ifndef _BOOKING_H_
#define _BOOKING_H_
struct Booking {
    int bookingID;
    int userID;
    int showTimeID;
    Booking(int bookingID, int userID, int showTimeID);
};
#endif  