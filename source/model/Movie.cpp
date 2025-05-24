#include "Movie.h"

Movie::Movie(int id, const std::string &title, const std::string &genre,
             const std::string &description, float rating)
    : id(id), title(title), genre(genre),
      description(description), rating(rating) {}

Movie::Movie(const std::string &title, const std::string &genre,
             const std::string &description, float rating) : title(title), genre(genre),
                                                             description(description), rating(rating) {}

int Movie::getId() const { return id; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getGenre() const { return genre; }
std::string Movie::getDescription() const { return description; }
float Movie::getRating() const { return rating; }
const std::vector<std::string> &Movie::getShowTimes() const { return showTimes; }
void Movie::addShowTime(const std::string &time) { showTimes.push_back(time); }
