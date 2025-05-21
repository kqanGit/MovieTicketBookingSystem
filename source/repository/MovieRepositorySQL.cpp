#include "MovieRepositorySQL.h"
#include "Movie.h"
#include "MovieMapper.h"

// MovieRepositorySQL::MovieRepositorySQL(std::shared_ptr<DatabaseConnection> conn) {
//     dbConn = conn.get();
// }

MovieRepositorySQL::MovieRepositorySQL(const std::string& filePath) {
    dbConn = DatabaseConnection::getInstance();
    if (!dbConn->connect(filePath)) {
        throw std::runtime_error("Failed to connect to database");
    }
}

std::vector<MovieDTO> MovieRepositorySQL::getAllMovies() {
    const std::string sql = "SELECT MovieID, Title, Genre, Rating FROM MOVIE";
    auto results = dbConn->executeQuery(sql);

    std::vector<MovieDTO> movies;
    for (const auto& row : results) {
        auto movie = std::make_shared<Movie>(
            std::stoi(row.at("MovieID")),
            row.at("Title"),
            row.at("Genre"),
            "", // description tạm bỏ qua 
            std::stof(row.at("Rating"))
        );
        movies.push_back(MovieMapper::toDTO(movie));
    }
    return movies;
}

std::shared_ptr<IMovie> MovieRepositorySQL::getMovieById(int id) {
    const std::string sql = "SELECT MovieID, Title, Genre, Descriptions, Rating FROM MOVIE WHERE MovieID = ?";
    auto results = dbConn->executeQuery(sql, {std::to_string(id)});

    if (results.empty()) {
        return nullptr;
    }

    const auto& row = results[0];
    return std::make_shared<Movie>(
        std::stoi(row.at("MovieID")),
        row.at("Title"),
        row.at("Genre"),
        row.at("Descriptions"),
        std::stof(row.at("Rating"))
    );
}

// Các hàm helper có thể thêm vào
bool isValidDate(const std::string& date) {
    // Kiểm tra định dạng YYYY-MM-DD
    std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
    return std::regex_match(date, pattern);
}

bool isValidTime(const std::string& time) {
    // Kiểm tra định dạng HH:MM:SS
    std::regex pattern(R"(\d{2}:\d{2}:\d{2})");
    return std::regex_match(time, pattern);
}

void MovieRepositorySQL::addShowTime(int movieId, std::string& Date, std::string& StartTime, std::string& EndTime) {
     // Validate định dạng ngày và giờ
    if (!isValidDate(Date) || !isValidTime(StartTime) || !isValidTime(EndTime)) {
        throw std::invalid_argument("Invalid date or time format");
    }

    // Kiểm tra thời gian kết thúc > thời gian bắt đầu
    if (EndTime <= StartTime) {
        throw std::invalid_argument("End time must be after start time");
    }

    // Câu lệnh SQL phù hợp với cấu trúc bảng
    const std::string sql = "INSERT INTO SHOWTIME (MovieID, Date, StartTime, EndTime) VALUES (?, ?, ?, ?)";
    
    bool success = dbConn->executeNonQuery(sql, {
        std::to_string(movieId),  // MovieID
        Date,                     // Date (YYYY-MM-DD)
        StartTime,                // StartTime (HH:MM:SS)
        EndTime                   // EndTime (HH:MM:SS)
    });

    if (!success) {
        throw std::runtime_error("Failed to add showtime to database");
    }
}



void MovieRepositorySQL::addMovie(std::shared_ptr<IMovie> movie) {
    const std::string sql = "INSERT INTO MOVIE (Title, Genre, Descriptions, Rating) "
                           "VALUES (?, ?, ?, ?, ?)";
    
    bool success = dbConn->executeNonQuery(sql, {
        movie->getTitle(),
        movie->getGenre(),
        movie->getDescription(),
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

MovieRepositorySQL::~MovieRepositorySQL() {
    // Don't delete dbConn since it's a singleton managed by DatabaseConnection class
}

std::vector<std::string> MovieRepositorySQL::getShowTimesByMovieId(int id) {
    const std::string sql = "SELECT Date, StartTime, EndTime FROM SHOWTIME WHERE MovieID = ?";
    auto results = dbConn->executeQuery(sql, {std::to_string(id)});
    
    std::vector<std::string> showTimes;
    for (const auto& row : results) {
        std::string showTime = row.at("Date") + ", " + row.at("StartTime") + ", " + row.at("EndTime");
        showTimes.push_back(showTime);
    }
    return showTimes;
}


void MovieRepositorySQL::deleteAllShowTimes(int movieId) {
    const std::string sql = "DELETE FROM SHOWTIME WHERE MovieID = ?";
    bool success = dbConn->executeNonQuery(sql, {std::to_string(movieId)});
    
    if (!success) {
        throw std::runtime_error("Failed to delete all showtimes from database");
    }
}


void MovieRepositorySQL::deleteShowTime(int movieId, int ShowTimeID) {
    const std::string sql = "DELETE FROM SHOWTIME WHERE MovieID = ? AND ShowTimeID = ?";
    bool success = dbConn->executeNonQuery(sql, {std::to_string(movieId), std::to_string(ShowTimeID)});
    
    if (!success) {
        throw std::runtime_error("Failed to delete showtime from database");
    }
}