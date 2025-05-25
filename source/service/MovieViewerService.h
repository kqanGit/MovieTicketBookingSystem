// MovieViewerService.h
#ifndef MOVIEVIEWERSERVICE_H
#define MOVIEVIEWERSERVICE_H

#include "IMovieViewerService.h"
#include "../repository/IMovieRepository.h"
#include <memory>
#include <vector>   
#include "../model/ShowTime.h"  // Include ShowTime model

class MovieViewerService : public IMovieViewerService {
private:
    std::shared_ptr<IMovieRepository> repo;  // Using smart pointer

public:
    explicit MovieViewerService(std::shared_ptr<IMovieRepository> r);
    std::vector<MovieDTO> showAllMovies() override;
    std::shared_ptr<IMovie> showMovieDetail(int id) override;
    std::vector<ShowTime> showMovieShowTimes(int id) override;  // Changed to return ShowTime vector
};

#endif // MOVIEVIEWERSERVICE_H