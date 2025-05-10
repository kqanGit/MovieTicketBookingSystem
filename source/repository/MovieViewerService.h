#ifndef MOVIEVIEWERSERVICE_H
#define MOVIEVIEWERSERVICE_H

#include "IMovieViewerService.h"
#include "IMovieRepository.h"

class MovieViewerService : public IMovieViewerService {
private:
    IMovieRepository* repo;

public:
    MovieViewerService(IMovieRepository* r);
    void showAllMovies() override;
    void showMovieDetail(int id) override;
};

#endif // MOVIEVIEWERSERVICE_H