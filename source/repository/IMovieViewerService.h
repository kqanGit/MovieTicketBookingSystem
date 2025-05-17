#ifndef IMOVIEVIEWERSERVICE_H
#define IMOVIEVIEWERSERVICE_H

#include <vector>
#include "MovieDTO.h"
#include "Movie.h"
#include <memory>

class IMovieViewerService {
public:
    virtual ~IMovieViewerService() {}
    virtual std::vector<MovieDTO> showAllMovies() = 0;
    virtual shared_ptr<IMovie> showMovieDetail(int id) = 0;
    virtual std::vector<std::string> showMovieShowTimes(int id) = 0;
};

#endif // IMOVIEVIEWERSERVICE_H