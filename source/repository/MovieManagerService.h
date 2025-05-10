#ifndef MOVIEMANAGERSERVICE_H
#define MOVIEMANAGERSERVICE_H

#include "IMovieManagerService.h"
#include "IMovieRepository.h"

class MovieManagerService : public IMovieManagerService {
private:
    IMovieRepository* repo;

public:
    MovieManagerService(IMovieRepository* r);
    void addMovie(IMovie* movie) override;
    void deleteMovie(int id) override;
};

#endif // MOVIEMANAGERSERVICE_H