#ifndef _COUPLESEAT_H_
#define _COUPLESEAT_H_
#include "ISeat.h"
#include <string>

class CoupleSeat : public ISeat {
private:
    std::string _seatID;
    float _price;
    SeatType _type;
public:
    CoupleSeat(std::string seatID, SeatType type, float price);
    float price() const override;
    std::string id() const override;
    SeatType type() const override;
};
#endif