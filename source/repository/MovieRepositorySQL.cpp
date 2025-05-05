#include "MovieRepositorySQL.h"

vector<MovieDTO> MovieRepositorySQL::getAllMovies() {
    vector<MovieDTO> dtos;
    for (auto& movie : db) {
        dtos.emplace_back(movie->getId(), movie->getTitle(), movie->getGenre(), movie->getRating());
    }
    return dtos;
}

IMovie* MovieRepositorySQL::getMovieById(int id) {
    for (auto& movie : db) {
        if (movie->getId() == id) return movie.get();
    }
    return nullptr;
}

void MovieRepositorySQL::addMovie(IMovie* m) {
    Movie* movie = dynamic_cast<Movie*>(m);
    if (movie)
        db.push_back(make_shared<Movie>(*movie));
}

void MovieRepositorySQL::deleteMovie(int id) {
    db.erase(remove_if(db.begin(), db.end(),
                       [id](shared_ptr<Movie> m) { return m->getId() == id; }),
             db.end());
}