#ifndef MOVIE_MANAGER_SERVICE_H
#define MOVIE_MANAGER_SERVICE_H

#include "Interfaces.h"
#include <iostream>
#include <string>

class MovieManagerService : public IMovieManagerService {
public:
    MovieManagerService() {
        std::cout << "MovieManagerService initialized" << std::endl;
    }
    
    void addMovie() {
        std::cout << "Admin functionality: Adding a new movie" << std::endl;
        std::cout << "Movie 'Godzilla vs. Kong' added successfully!" << std::endl;
    }
    
    void updateMovie() {
        std::cout << "Admin functionality: Updating a movie" << std::endl;
        std::cout << "Movie ID 2 (The Matrix Resurrections) updated successfully!" << std::endl;
    }
    
    void deleteMovie() {
        std::cout << "Admin functionality: Deleting a movie" << std::endl;
        std::cout << "Movie ID 5 (Eternals) deleted successfully!" << std::endl;
    }
    
    void listMovies() {
        std::cout << "Admin functionality: Listing all movies" << std::endl;
        std::cout << "1. Spider-Man: No Way Home (ID: 1)" << std::endl;
        std::cout << "2. The Matrix Resurrections (ID: 2)" << std::endl;
        std::cout << "3. Dune (ID: 3)" << std::endl;
        std::cout << "4. No Time to Die (ID: 4)" << std::endl;
    }
};

#endif // MOVIE_MANAGER_SERVICE_H