// MovieViewerService.h
#ifndef MOVIEVIEWERSERVICE_H
#define MOVIEVIEWERSERVICE_H

#include "IMovieViewerService.h"
#include "IMovieRepository.h"
#include <memory>
#include <vector>   
// #include "MovieDTO.h"
// #include "Movie.h"  // already included in IMovieViewerService.h

class MovieViewerService : public IMovieViewerService {
private:
    std::shared_ptr<IMovieRepository> repo;  // Using smart pointer

public:
    explicit MovieViewerService(std::shared_ptr<IMovieRepository> r);
    std::vector<MovieDTO> showAllMovies() override;
    std::shared_ptr<IMovie> showMovieDetail(int id) override;
    std::vector<std::string> showMovieShowTimes(int id) override;
};

#endif // MOVIEVIEWERSERVICE_H