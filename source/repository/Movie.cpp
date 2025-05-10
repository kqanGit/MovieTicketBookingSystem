#include "Movie.h"

Movie::Movie(int id, const string& title, const string& genre, int duration,
             const string& description, float rating)
    : id(id), title(title), genre(genre), duration(duration),
      description(description), rating(rating) {}

int Movie::getId() const { return id; }
string Movie::getTitle() const { return title; }
string Movie::getGenre() const { return genre; }
int Movie::getDuration() const { return duration; }
string Movie::getDescription() const { return description; }
float Movie::getRating() const { return rating; }
const vector<string>& Movie::getShowTimes() const { return showTimes; }
void Movie::addShowTime(const string& time) { showTimes.push_back(time); }
