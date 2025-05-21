#ifndef _SINGLESEAT_H_
#define _SINGLESEAT_H_
#include "ISeat.h"
#include <string>

class SingleSeat : public ISeat {
private:
    std::string _seatID;
    float _price;
    SeatType _type;
public:
    SingleSeat(std::string seatID, SeatType type, float price);
    float price() const override;
    SeatType type() const override;
    std::string id() const override;
};
#endif 