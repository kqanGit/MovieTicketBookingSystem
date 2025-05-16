#ifndef MOVIE_H
#define MOVIE_H

#include "IMovie.h"
#include <vector>

class Movie : public IMovie {
private:
    int id;
    string title;
    string genre;
    int duration;
    string description;
    float rating;
    vector<string> showTimes;

public:
    Movie(){}
    Movie(int id, const string& title, const string& genre, int duration,
          const string& description, float rating);

    int getId() const override;
    string getTitle() const override;
    string getGenre() const override;
    int getDuration() const override;
    string getDescription() const override;
    float getRating() const override;

    const vector<string>& getShowTimes() const;
    void addShowTime(const string& time);

    void setId(int _id) { id = _id; }
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setGenre(const std::string& newGenre) { genre = newGenre; }
    void setRating(float newRating) { rating = newRating; }
    void setDuration(int newDuration) { duration = newDuration; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
};

#endif // MOVIE_H