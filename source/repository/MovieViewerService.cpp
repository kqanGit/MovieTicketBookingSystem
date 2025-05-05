#include "MovieViewerService.h"
#include <iostream>
using namespace std;

MovieViewerService::MovieViewerService(IMovieRepository* r) : repo(r) {}

void MovieViewerService::showAllMovies() {
    auto movies = repo->getAllMovies();
    for (auto& m : movies) {
        cout << m.id << ": " << m.title << " (" << m.genre << ") - Rating: " << m.rating << endl;
    }
}

void MovieViewerService::showMovieDetail(int id) {
    IMovie* m = repo->getMovieById(id);
    if (m) {
        cout << "ID: " << m->getId() << endl;
        cout << "Title: " << m->getTitle() << endl;
        cout << "Genre: " << m->getGenre() << endl;
        cout << "Duration: " << m->getDuration() << " mins\n";
        cout << "Description: " << m->getDescription() << endl;
        cout << "Rating: " << m->getRating() << endl;
    } else {
        cout << "Movie not found.\n";
    }
}