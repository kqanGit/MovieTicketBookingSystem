#ifndef IMOVIEREPOSITORY_H
#define IMOVIEREPOSITORY_H

#include <vector>
#include "MovieDTO.h"
#include "IMovie.h"

class IMovieRepository {
public:
    virtual ~IMovieRepository() {}
    virtual vector<MovieDTO> getAllMovies() = 0;
    virtual std::shared_ptr<IMovie> getMovieById(int id) = 0;
    virtual void addMovie(std::shared_ptr<IMovie> movie) = 0;
    virtual void deleteMovie(int id) = 0;
};

#endif // IMOVIEREPOSITORY_H