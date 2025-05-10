#ifndef _SINGLESEAT_H_
#define _SINGLESEAT_H_
#include "ISeat.h"
#include <string>
class SingleSeat : public ISeat {
private:
    std::string _seatID;
    float _price;
public:
    SingleSeat(int seatID, float price);
};
#endif 