#ifndef IMOVIEREPOSITORY_H
#define IMOVIEREPOSITORY_H

#include <vector>
#include "MovieDTO.h"
#include "../model/IMovie.h"
#include <memory>
#include <string>
#include <iostream>


class IMovieRepository {
public:
    virtual ~IMovieRepository() {}
    virtual std::vector<MovieDTO> getAllMovies() = 0;
    virtual std::shared_ptr<IMovie> getMovieById(int id) = 0;
    virtual void addMovie(std::shared_ptr<IMovie> movie) = 0;
    virtual void deleteMovie(int id) = 0;
    virtual void addShowTime(int movieId, std::string& Date, std::string& StartTime, std::string& EndTime) = 0;
    virtual void deleteShowTime(int movieId, int ShowTimeId) = 0;
    virtual std::vector<std::string> getShowTimesByMovieId(int id) = 0;
    virtual void deleteAllShowTimes(int movieId) = 0;
};

#endif // IMOVIEREPOSITORY_H