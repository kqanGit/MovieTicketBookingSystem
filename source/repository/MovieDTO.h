#ifndef MOVIEDTO_H
#define MOVIEDTO_H
#include <iostream>
#include <memory>       
#include <string>


class MovieDTO {
public:
    int id;
    std::string title;
    std::string genre;
    float rating;
      // Add default constructor for convenience
    MovieDTO() : id(0), rating(0.0f) {}
    MovieDTO(int id, const std::string& title, const std::string& genre, float rating);
    
    // Add comparison method for use in tests
    bool operator==(const MovieDTO& other) const {
        return id == other.id && title == other.title 
            && genre == other.genre && rating == other.rating;
    }
};

#endif // MOVIEDTO_H