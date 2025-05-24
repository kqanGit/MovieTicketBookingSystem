#ifndef _SEATVIEW_H_
#define _SEATVIEW_H_
#include "../model/ISeat.h"
#include <memory>
enum SeatStatus {
    AVAILABLE,
    BOOKED,
};

class SeatView {
public:
    std::shared_ptr<ISeat> seat;
    SeatStatus status;
public:
    SeatView(std::shared_ptr<ISeat> seat, const SeatStatus& status);
};

#endif 