#ifndef IMOVIEVIEWERSERVICE_H
#define IMOVIEVIEWERSERVICE_H

class IMovieViewerService {
public:
    virtual ~IMovieViewerService() {}
    virtual void showAllMovies() = 0;
    virtual void showMovieDetail(int id) = 0;
};

#endif // IMOVIEVIEWERSERVICE_H