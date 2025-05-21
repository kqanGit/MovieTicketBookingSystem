#ifndef MOVIE_H
#define MOVIE_H

#include "IMovie.h"
#include <vector>
#include <string>

class Movie : public IMovie
{
private:
    int id;
    std::string title;
    std::string genre;
    std::string description;
    float rating;
    std::vector<std::string> showTimes;

public:
    Movie() {}
    Movie(int id, const std::string &title, const std::string &genre,
          const std::string &description, float rating);
    Movie(const std::string &title, const std::string &genre,
          const std::string &description, float rating);

    int getId() const override;
    std::string getTitle() const override;
    std::string getGenre() const override;
    std::string getDescription() const override;
    float getRating() const override;

    const std::vector<std::string> &getShowTimes() const;
    void addShowTime(const std::string &time);

    void setId(int _id) { id = _id; }
    void setTitle(const std::string &newTitle) { title = newTitle; }
    void setGenre(const std::string &newGenre) { genre = newGenre; }
    void setRating(float newRating) { rating = newRating; }
    void setDescription(const std::string &newDescription) { description = newDescription; }
};

#endif // MOVIE_H