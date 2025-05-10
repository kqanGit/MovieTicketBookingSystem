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
};

#endif // MOVIE_H