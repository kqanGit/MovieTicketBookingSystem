// MovieManagerService.h
#ifndef MOVIEMANAGERSERVICE_H
#define MOVIEMANAGERSERVICE_H

#include "IMovieManagerService.h"
#include "IMovieRepository.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class MovieManagerService : public IMovieManagerService {
private:
    std::shared_ptr<IMovieRepository> repo;

public:
    explicit MovieManagerService(std::shared_ptr<IMovieRepository> r);
    void addMovie(std::shared_ptr<IMovie> movie, std::vector<std::string> ShowTimes) override;
    void deleteMovie(int id) override;
    void deleteShowTime(int movieId, int ShowTimeId) override;
};

#endif // MOVIEMANAGERSERVICE_H