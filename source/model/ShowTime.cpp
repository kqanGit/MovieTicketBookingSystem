#include "ShowTime.h"

ShowTime::ShowTime(int showTimeID, int movieID, const std::string& date, const std::string& startTime, const std::string& endTime)
    : showTimeID(showTimeID), movieID(movieID), date(date), startTime(startTime), endTime(endTime) {}