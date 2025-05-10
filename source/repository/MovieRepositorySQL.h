#ifndef MOVIEREPOSITORYSQL_H
#define MOVIEREPOSITORYSQL_H

#include "IMovieRepository.h"
#include "Movie.h"
#include <vector>
#include <memory>

class MovieRepositorySQL : public IMovieRepository {
private:
    vector<shared_ptr<Movie>> db;

public:
    vector<MovieDTO> getAllMovies() override;
    IMovie* getMovieById(int id) override;
    void addMovie(IMovie* movie) override;
    void deleteMovie(int id) override;
};

#endif