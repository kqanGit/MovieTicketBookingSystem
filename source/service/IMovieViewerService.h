#ifndef IMOVIEVIEWERSERVICE_H
#define IMOVIEVIEWERSERVICE_H

#include <vector>
#include "../repository/MovieDTO.h"
#include "../model/Movie.h"
#include "../model/ShowTime.h"
#include <memory>

class IMovieViewerService {
public:    
    virtual ~IMovieViewerService() {}
    virtual std::vector<MovieDTO> showAllMovies() = 0;
    virtual std::shared_ptr<IMovie> showMovieDetail(int id) = 0;
    virtual std::vector<ShowTime> showMovieShowTimes(int id) = 0;
};

#endif // IMOVIEVIEWERSERVICE_H