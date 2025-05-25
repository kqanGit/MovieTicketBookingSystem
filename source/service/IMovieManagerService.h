#ifndef IMOVIEMANAGERSERVICE_H
#define IMOVIEMANAGERSERVICE_H

#include "../model/IMovie.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class IMovieManagerService {
public:
    virtual ~IMovieManagerService() {}
    virtual void addMovie(std::shared_ptr<IMovie> movie, std::vector<std::string> ShowTimes) = 0;
    virtual void deleteMovie(int id) = 0;
    virtual void deleteShowTime(int movieId, int ShowTimeId) = 0;
};

#endif // IMOVIEMANAGERSERVICE_H