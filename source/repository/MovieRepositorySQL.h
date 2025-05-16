// MovieRepositorySQL.h
#ifndef MOVIEREPOSITORYSQL_H
#define MOVIEREPOSITORYSQL_H

#include "IMovieRepository.h"
#include "DatabaseConnection.h"
#include <memory>

class MovieRepositorySQL : public IMovieRepository {
private:
    std::shared_ptr<DatabaseConnection> dbConn;

public:
    explicit MovieRepositorySQL(std::shared_ptr<DatabaseConnection> conn);
    std::vector<MovieDTO> getAllMovies() override;
    std::shared_ptr<IMovie> getMovieById(int id) override;
    void addMovie(std::shared_ptr<IMovie> movie) override;
    void deleteMovie(int id) override;
};

#endif // MOVIEREPOSITORYSQL_H