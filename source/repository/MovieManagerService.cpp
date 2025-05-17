#include "MovieManagerService.h"

MovieManagerService::MovieManagerService(std::shared_ptr<IMovieRepository> r) : repo(r) {}

void MovieManagerService::addMovie(std::shared_ptr<IMovie> movie, std::vector<std::string> ShowTimes) {
    repo->addMovie(movie);
    for (const auto& showTime : ShowTimes) {
        std::istringstream ss(showTime);
        std::string date, startTime, endTime;
        std::getline(ss, date, ',');
        std::getline(ss, startTime, ',');
        std::getline(ss, endTime);
        
        repo->addShowTime(movie->getId(), date, startTime, endTime);
    }
}

void MovieManagerService::deleteMovie(int id) {
    repo->deleteMovie(id);

    repo->deleteAllShowTimes(id);
}

void MovieManagerService::deleteShowTime(int movieId, int ShowTimeId) {
    repo->deleteShowTime(movieId, ShowTimeId);
}