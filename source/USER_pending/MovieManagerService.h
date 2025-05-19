#ifndef MOVIE_MANAGER_SERVICE_H
#define MOVIE_MANAGER_SERVICE_H

#include "IMovieManagerService.h"
#include <iostream>

class MovieManagerService : public IMovieManagerService {
public:
    MovieManagerService() {
        std::cout << "[Service] MovieManagerService initialized.\n";
    }

    void addMovie() {
        std::cout << "[Admin] Adding a new movie...\n";
        std::cout << "Movie 'Dune: Part Two' added successfully!\n";
    }

    void updateMovie() {
        std::cout << "[Admin] Updating movie info...\n";
        std::cout << "Movie 'The Matrix Resurrections' updated.\n";
    }

    void deleteMovie() {
        std::cout << "[Admin] Deleting movie...\n";
        std::cout << "Movie ID 3 deleted from system.\n";
    }
};

#endif // MOVIE_MANAGER_SERVICE_H
