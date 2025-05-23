#include "CoupleSeat.h"

CoupleSeat::CoupleSeat(std::string seatID, SeatType type, float price) : _seatID(seatID), _type(type),_price(price) {}

float CoupleSeat::price() const {
    return _price;
}

SeatType CoupleSeat::type() const {
    return _type;
}

std::string CoupleSeat::id() const {
    return _seatID;
}