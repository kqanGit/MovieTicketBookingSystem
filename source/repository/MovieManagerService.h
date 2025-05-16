// MovieManagerService.h
#ifndef MOVIEMANAGERSERVICE_H
#define MOVIEMANAGERSERVICE_H

#include "IMovieManagerService.h"
#include "IMovieRepository.h"
#include <memory>

class MovieManagerService : public IMovieManagerService {
private:
    std::shared_ptr<IMovieRepository> repo;

public:
    explicit MovieManagerService(std::shared_ptr<IMovieRepository> r);
    void addMovie(std::shared_ptr<IMovie> movie) override;
    void deleteMovie(int id) override;
};

#endif // MOVIEMANAGERSERVICE_H