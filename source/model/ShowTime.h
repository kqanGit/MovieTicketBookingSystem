#ifndef _SHOWTIME_H_
#define _SHOWTIME_H_
#include <string>
struct ShowTime {
    int showTimeID;
    std::string date;
    std::string startTime;
    std::string endTime;

    ShowTime(int showTimeID, const std::string& date, const std::string& startTime, const std::string& endTime);
};

#endif