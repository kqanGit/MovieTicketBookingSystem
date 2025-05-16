// MovieViewerService.h
#ifndef MOVIEVIEWERSERVICE_H
#define MOVIEVIEWERSERVICE_H

#include "IMovieViewerService.h"
#include "IMovieRepository.h"
#include <memory>

class MovieViewerService : public IMovieViewerService {
private:
    std::shared_ptr<IMovieRepository> repo;  // Thay đổi thành smart pointer

public:
    explicit MovieViewerService(std::shared_ptr<IMovieRepository> r);
    void showAllMovies() override;
    void showMovieDetail(int id) override;
};

#endif // MOVIEVIEWERSERVICE_H