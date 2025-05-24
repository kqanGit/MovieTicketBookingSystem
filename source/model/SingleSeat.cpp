#include "SingleSeat.h"

SingleSeat::SingleSeat(std::string seatID, SeatType type, float price) : _seatID(seatID), _type(type), _price(price) {}

float SingleSeat::price() const {
    return _price;
}

SeatType SingleSeat::type() const {
    return _type;
}

std::string SingleSeat::id() const {
    return _seatID;
}