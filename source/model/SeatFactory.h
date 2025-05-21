#ifndef _SEATFACTORY_H_
#define _SEATFACTORY_H_
#include "ISeat.h"
#include "SingleSeat.h"
#include "CoupleSeat.h"
#include <string>

class SeatFactory {
private:

public:
    ISeat* createSeat(std::string seatID, SeatType type, float price) {
        if (type == SINGLE) {
            return new SingleSeat(seatID, type, price);
        }
        else if (type == COUPLE) {
            return new CoupleSeat(seatID, type, price);
        }
        return nullptr; // Hoặc ném ngoại lệ nếu không tìm thấy loại ghế
    }
};
#endif