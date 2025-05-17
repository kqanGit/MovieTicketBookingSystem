// MovieViewerService.h
#ifndef MOVIEVIEWERSERVICE_H
#define MOVIEVIEWERSERVICE_H

#include "IMovieViewerService.h"
#include "IMovieRepository.h"
#include <memory>
#include <vector>   
// #include "MovieDTO.h"
// #include "Movie.h"  // đã include trong file IMovieViewerService.h

class MovieViewerService : public IMovieViewerService {
private:
    std::shared_ptr<IMovieRepository> repo;  // Thay đổi thành smart pointer

public:
    explicit MovieViewerService(std::shared_ptr<IMovieRepository> r);
    std::vector<MovieDTO> showAllMovies() override;
    shared_ptr<IMovie> showMovieDetail(int id) override;
    vector<std::string> showMovieShowTimes(int id) override;
};

#endif // MOVIEVIEWERSERVICE_H