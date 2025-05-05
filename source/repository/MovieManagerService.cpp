#include "MovieManagerService.h"

MovieManagerService::MovieManagerService(IMovieRepository* r) : repo(r) {}
void MovieManagerService::addMovie(IMovie* movie) {
    repo->addMovie(movie);
}
void MovieManagerService::deleteMovie(int id) {
    repo->deleteMovie(id);
}
