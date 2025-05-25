#include "MovieManagerService.h"

MovieManagerService::MovieManagerService(std::shared_ptr<IMovieRepository> r) : repo(r) {}

void MovieManagerService::addMovie(std::shared_ptr<IMovie> movie, std::vector<std::string> ShowTimes) {
    int newMovieId = repo->addMovie(movie); // Get the new movie ID
    // movie->setId(newMovieId); // Optionally set the ID back on the movie object if needed by it

    for (const auto& showTime : ShowTimes) {
        std::istringstream ss(showTime);
        std::string date, startTime, endTime;
        std::getline(ss, date, ',');
        std::getline(ss, startTime, ',');
        std::getline(ss, endTime);
        
        // std::cout << "[MovieManagerService[DEBUG]] Adding showtime for movie ID " << newMovieId 
        //      << ": Date: " << date << ", Start: " << startTime << ", End: " << endTime << std::endl;

        repo->addShowTime(newMovieId, date, startTime, endTime);
    }
}

void MovieManagerService::deleteMovie(int id) {
    repo->deleteMovie(id);

    repo->deleteAllShowTimes(id);
}

void MovieManagerService::deleteShowTime(int movieId, int ShowTimeId) {
    repo->deleteShowTime(movieId, ShowTimeId);
}