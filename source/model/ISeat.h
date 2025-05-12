#ifndef _ISEAT_H_
#define _ISEAT_H_
#include <string>

enum SeatType{
    SINGLE, 
    COUPLE
};

class ISeat {
public:
    virtual ~ISeat() = default;
    virtual float price() const = 0;
    virtual std::string id() const = 0;
    virtual SeatType type() const = 0;
};

#endif 