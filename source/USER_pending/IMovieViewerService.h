#ifndef IMOVIEVIEWERSERVICE_H
#define IMOVIEVIEWERSERVICE_H

class IMovieViewerService {
public:
    virtual ~IMovieViewerService() = default;
    virtual void viewMovieList() = 0;
    virtual void viewMovieDetails(int movieId) = 0;
    virtual void searchMovies(const std::string& query) = 0;
};


#endif
