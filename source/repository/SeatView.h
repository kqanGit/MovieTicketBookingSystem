#ifndef _SEATVIEW_H_
#define _SEATVIEW_H_
#include "ISeat.h"

enum SeatStatus {
    AVAILABLE,
    BOOKED,
};

class SeatView {
public:
    ISeat* seat;
    SeatStatus status;
public:
    SeatView(ISeat* seat, const SeatStatus& status);
};

#endif 