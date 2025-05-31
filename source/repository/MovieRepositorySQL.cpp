#include "MovieRepositorySQL.h"
#include "Movie.h"
#include "MovieMapper.h"
#include "../model/ShowTime.h"

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
    // Kiểm tra định dạng HH:MM:SS hoặc HH:MM
    std::regex patternWithSeconds(R"(\d{2}:\d{2}:\d{2})");
    std::regex patternWithoutSeconds(R"(\d{2}:\d{2})");
    return std::regex_match(time, patternWithSeconds) || std::regex_match(time, patternWithoutSeconds);
}

void MovieRepositorySQL::addShowTime(int movieId, std::string& Date, std::string& StartTime, std::string& EndTime) {
    // Tự động thêm giây nếu định dạng là HH:MM
    auto addSeconds = [](std::string& time) {
        if (std::regex_match(time, std::regex(R"(\d{2}:\d{2})"))) {
            time += ":00";
        }
    };
    
    addSeconds(StartTime);
    addSeconds(EndTime);
    
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



int MovieRepositorySQL::addMovie(std::shared_ptr<IMovie> movie) {
    const std::string sql = "INSERT INTO MOVIE (Title, Genre, Descriptions, Rating) "
                           "VALUES (?, ?, ?, ?)";
    
    bool success = dbConn->executeNonQuery(sql, {
        movie->getTitle(),
        movie->getGenre(),
        movie->getDescription(),
        std::to_string(movie->getRating())
    });


    if (!success) {
        throw std::runtime_error("Failed to add movie to database");
    }
    // Retrieve the last inserted ID
    // This is specific to SQLite. Other databases might have different ways.
    auto result = dbConn->executeQuery("SELECT last_insert_rowid();");
    if (!result.empty() && result[0].count("last_insert_rowid()")) {
        return std::stoi(result[0].at("last_insert_rowid()"));
    }
    throw std::runtime_error("Failed to retrieve last inserted movie ID.");
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

std::vector<ShowTime> MovieRepositorySQL::getShowTimesByMovieId(int id) {
    const std::string sql = "SELECT ShowTimeID, Date, StartTime, EndTime FROM SHOWTIME WHERE MovieID = ?";
    auto results = dbConn->executeQuery(sql, {std::to_string(id)});
    
    std::vector<ShowTime> showTimes;
    for (const auto& row : results) {
        int showTimeID = std::stoi(row.at("ShowTimeID"));
        std::string date = row.at("Date");
        std::string startTime = row.at("StartTime");
        std::string endTime = row.at("EndTime");
        showTimes.emplace_back(showTimeID, date, startTime, endTime);
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