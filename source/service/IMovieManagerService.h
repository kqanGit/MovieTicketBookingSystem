#ifndef IMOVIEMANAGERSERVICE_H
#define IMOVIEMANAGERSERVICE_H

class IMovieManagerService {
public:
    virtual ~IMovieManagerService() = default;
    virtual void addMovie() = 0;
    virtual void updateMovie() = 0;
    virtual void deleteMovie() = 0;
};


#endif
