#include "Movie.h"
#include "MovieRepositorySQL.h"
#include "MovieViewerService.h"
#include "MovieManagerService.h"

int main() {
    MovieRepositorySQL repo;

    MovieManagerService manager(&repo);
    MovieViewerService viewer(&repo);

    Movie m1(1, "Inception", "Sci-Fi", 148, "A mind-bending thriller.", 8.8);
    Movie m2(2, "Interstellar", "Sci-Fi", 169, "Space exploration and time dilation.", 9.0);

    manager.addMovie(&m1);
    manager.addMovie(&m2);

    viewer.showAllMovies();
    viewer.showMovieDetail(2);

    return 0;
}
