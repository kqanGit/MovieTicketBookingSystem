#ifndef MOVIEDTO_H
#define MOVIEDTO_H

#include <string>
using namespace std;

class MovieDTO {
public:
    int id;
    string title;
    string genre;
    float rating;

    MovieDTO(int id, const string& title, const string& genre, float rating);
};

#endif // MOVIEDTO_H