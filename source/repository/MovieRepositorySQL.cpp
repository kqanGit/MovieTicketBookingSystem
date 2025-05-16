#include "MovieRepositorySQL.h"
#include "Movie.h"
#include "MovieMapper.h"

MovieRepositorySQL::MovieRepositorySQL(std::shared_ptr<DatabaseConnection> conn) 
    : dbConn(conn) {}

std::vector<MovieDTO> MovieRepositorySQL::getAllMovies() {
    const std::string sql = "SELECT MovieID, Title, Genre, Rating FROM MOVIE";
    auto results = dbConn->executeQuery(sql);

    std::vector<MovieDTO> movies;
    for (const auto& row : results) {
        auto movie = std::make_shared<Movie>(
            std::stoi(row.at("MovieID")),
            row.at("Title"),
            row.at("Genre"),
            0, // duration tạm bỏ qua
            "", // description tạm bỏ qua 
            std::stof(row.at("Rating"))
        );
        movies.push_back(MovieMapper::toDTO(movie));
    }
    return movies;
}

std::shared_ptr<IMovie> MovieRepositorySQL::getMovieById(int id) {
    const std::string sql = "SELECT MovieID, Title, Genre, Descriptions, Duration, Rating FROM MOVIE WHERE MovieID = ?";
    auto results = dbConn->executeQuery(sql, {std::to_string(id)});

    if (results.empty()) {
        return nullptr;
    }

    const auto& row = results[0];
    return std::make_shared<Movie>(
        std::stoi(row.at("MovieID")),
        row.at("Title"),
        row.at("Genre"),
        std::stoi(row.at("Duration")),
        row.at("Descriptions"),
        std::stof(row.at("Rating"))
    );
}

void MovieRepositorySQL::addMovie(std::shared_ptr<IMovie> movie) {
    const std::string sql = "INSERT INTO MOVIE (MovieID, Title, Genre, Descriptions, Duration, Rating) "
                           "VALUES (?, ?, ?, ?, ?, ?)";
    
    bool success = dbConn->executeNonQuery(sql, {
        std::to_string(movie->getId()),
        movie->getTitle(),
        movie->getGenre(),
        movie->getDescription(),
        std::to_string(movie->getDuration()),
        std::to_string(movie->getRating())
    });

    if (!success) {
        throw std::runtime_error("Failed to add movie to database");
    }
}

void MovieRepositorySQL::deleteMovie(int id) {
    const std::string sql = "DELETE FROM MOVIE WHERE MovieID = ?";
    bool success = dbConn->executeNonQuery(sql, {std::to_string(id)});
    
    if (!success) {
        throw std::runtime_error("Failed to delete movie from database");
    }
}