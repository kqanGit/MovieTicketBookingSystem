#include "MovieViewerService.h"
#include <iostream>
using namespace std;

MovieViewerService::MovieViewerService(std::shared_ptr<IMovieRepository> r) : repo(move(r)) {}

std::vector<MovieDTO> MovieViewerService::showAllMovies() {
    auto movies = repo->getAllMovies();
    // for (auto& m : movies) {
    //     cout << m.id << ": " << m.title << " (" << m.genre << ") - Rating: " << m.rating << endl;
    // }
    return movies;
}

shared_ptr<IMovie> MovieViewerService::showMovieDetail(int id) {
    shared_ptr<IMovie> m = repo->getMovieById(id); // shared_ptr
    if (m) {
        // cout << "ID: " << m->getId() << endl;
        // cout << "Title: " << m->getTitle() << endl;
        // cout << "Genre: " << m->getGenre() << endl;
        // cout << "Duration: " << m->getDuration() << " mins\n";
        // cout << "Description: " << m->getDescription() << endl;
        // cout << "Rating: " << m->getRating() << endl;
        return m;
    } else {
        cout << "Movie not found.\n";
        return nullptr;
    }
}

vector<std::string> MovieViewerService::showMovieShowTimes(int id) {
    auto showTimes = repo->getShowTimesByMovieId(id);
    // for (const auto& st : showTimes) {
    //     cout << "Date: " << st.date << ", Start: " << st.startTime << ", End: " << st.endTime << endl;
    // }
    return showTimes;
}