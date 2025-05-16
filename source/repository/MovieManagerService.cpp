#include "MovieManagerService.h"

MovieManagerService::MovieManagerService(std::shared_ptr<IMovieRepository> r) : repo(r) {}

void MovieManagerService::addMovie(std::shared_ptr<IMovie> movie) {
    repo->addMovie(movie);
}
void MovieManagerService::deleteMovie(int id) {
    repo->deleteMovie(id);
}
