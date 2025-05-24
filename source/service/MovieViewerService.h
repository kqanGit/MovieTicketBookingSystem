#ifndef MOVIE_VIEWER_SERVICE_H
#define MOVIE_VIEWER_SERVICE_H

#include "IMovieViewerService.h"
#include <iostream>

class MovieViewerService : public IMovieViewerService {
public:
    MovieViewerService() {
        std::cout << "MovieViewerService initialized" << std::endl;
    }
    
    void viewMovieList() {
        std::cout << "Functionality: Viewing list of movies" << std::endl;
        std::cout << "1. Spider-Man: No Way Home (150 min)" << std::endl;
        std::cout << "2. The Matrix Resurrections (148 min)" << std::endl;
        std::cout << "3. Dune (155 min)" << std::endl;
        std::cout << "4. No Time to Die (163 min)" << std::endl;
        std::cout << "5. Eternals (157 min)" << std::endl;
    }
    
    void viewMovieDetails(int movieId) {
        std::cout << "Functionality: Viewing details of movie #" << movieId << std::endl;
        
        // Simulate movie details
        switch(movieId) {
            case 1:
                std::cout << "Title: Spider-Man: No Way Home" << std::endl;
                std::cout << "Duration: 150 minutes" << std::endl;
                std::cout << "Genre: Action, Adventure, Fantasy" << std::endl;
                std::cout << "Description: Peter Parker's identity as Spider-Man is revealed." << std::endl;
                break;
            case 2:
                std::cout << "Title: The Matrix Resurrections" << std::endl;
                std::cout << "Duration: 148 minutes" << std::endl;
                std::cout << "Genre: Action, Sci-Fi" << std::endl;
                std::cout << "Description: Neo returns to the Matrix to fight a new threat." << std::endl;
                break;
            default:
                std::cout << "Movie details not available for ID " << movieId << std::endl;
                break;
        }
    }
    
    void searchMovies(const std::string& query) {
        std::cout << "Functionality: Searching for movies with query: '" << query << "'" << std::endl;
        std::cout << "Search results for '" << query << "': (Simulated)" << std::endl;
        
        if (query == "action") {
            std::cout << "1. Spider-Man: No Way Home" << std::endl;
            std::cout << "2. The Matrix Resurrections" << std::endl;
            std::cout << "4. No Time to Die" << std::endl;
        } else {
            std::cout << "No movies found matching your search." << std::endl;
        }
    }
};

#endif // MOVIE_VIEWER_SERVICE_H