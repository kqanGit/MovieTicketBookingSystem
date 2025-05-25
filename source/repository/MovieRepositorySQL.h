// MovieRepositorySQL.h
#ifndef MOVIEREPOSITORYSQL_H
#define MOVIEREPOSITORYSQL_H

#include "../repository/IMovieRepository.h"
#include "../database/DatabaseConnection.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <regex>

class MovieRepositorySQL : public IMovieRepository {
private:
    DatabaseConnection* dbConn;

public:
    //explicit MovieRepositorySQL(std::shared_ptr<DatabaseConnection> conn);
    explicit MovieRepositorySQL(const std::string& filePath);
    

    std::vector<MovieDTO> getAllMovies() override;
    std::shared_ptr<IMovie> getMovieById(int id) override;
    void addMovie(std::shared_ptr<IMovie> movie) override;
    void deleteMovie(int id) override;
    void addShowTime(int movieId, std::string& Date, std::string& StartTime, std::string& EndTime) override;
    void deleteShowTime(int movieId, int ShowTimeID) override;
    std::vector<std::string> getShowTimesByMovieId(int id) override;
    void deleteAllShowTimes(int movieId) override;

    ~MovieRepositorySQL();
};

    bool isValidDate(const std::string &date);
    bool isValidTime(const std::string &time);

#endif // MOVIEREPOSITORYSQL_H