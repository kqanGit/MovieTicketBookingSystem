#ifndef IMOVIEMANAGERSERVICE_H
#define IMOVIEMANAGERSERVICE_H

#include "IMovie.h"

class IMovieManagerService {
public:
    virtual ~IMovieManagerService() {}
    virtual void addMovie(IMovie* movie) = 0;
    virtual void deleteMovie(int id) = 0;
};

#endif // IMOVIEMANAGERSERVICE_H