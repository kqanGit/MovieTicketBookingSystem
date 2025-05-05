#ifndef IMOVIEREPOSITORY_H
#define IMOVIEREPOSITORY_H

#include <vector>
#include "MovieDTO.h"
#include "IMovie.h"

class IMovieRepository {
public:
    virtual ~IMovieRepository() {}
    virtual vector<MovieDTO> getAllMovies() = 0;
    virtual IMovie* getMovieById(int id) = 0;
    virtual void addMovie(IMovie* movie) = 0;
    virtual void deleteMovie(int id) = 0;
};

#endif // IMOVIEREPOSITORY_H