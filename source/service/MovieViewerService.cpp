#include "MovieViewerService.h"
#include <iostream>

MovieViewerService::MovieViewerService(std::shared_ptr<IMovieRepository> r) : repo(std::move(r)) {}

std::vector<MovieDTO> MovieViewerService::showAllMovies() {
    auto movies = repo->getAllMovies();
    // for (auto& m : movies) {
    //     cout << m.id << ": " << m.title << " (" << m.genre << ") - Rating: " << m.rating << endl;
    // }
    return movies;
}

std::shared_ptr<IMovie> MovieViewerService::showMovieDetail(int id) {
    std::shared_ptr<IMovie> m = repo->getMovieById(id);
    if (m) {
        // cout << "ID: " << m->getId() << endl;
        // cout << "Title: " << m->getTitle() << endl;
        // cout << "Genre: " << m->getGenre() << endl;
        // cout << "Duration: " << m->getDuration() << " mins\n";
        // cout << "Description: " << m->getDescription() << endl;
        // cout << "Rating: " << m->getRating() << endl;
        return m;
    } else {
        std::cout << "Movie not found.\n";
        return nullptr;
    }
}

std::vector<std::string> MovieViewerService::showMovieShowTimes(int id) {
    auto showTimes = repo->getShowTimesByMovieId(id);
    // for (const auto& st : showTimes) {
    //     cout << "Date: " << st.date << ", Start: " << st.startTime << ", End: " << st.endTime << endl;
    // }
    return showTimes;
}