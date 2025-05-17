#include "MovieDTO.h"

MovieDTO::MovieDTO(int id, const std::string& title, const std::string& genre, float rating)
    : id(id), title(title), genre(genre), rating(rating) {}
