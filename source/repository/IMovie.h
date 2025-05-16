#ifndef IMOVIE_H
#define IMOVIE_H

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class IMovie {
    public:
    virtual ~IMovie() {}
    
    // Getter
    virtual int getId() const = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getGenre() const = 0;
    virtual int getDuration() const = 0;
    virtual std::string getDescription() const = 0;
    virtual float getRating() const = 0;

    // Setter
    virtual void setId(int id) = 0;
    virtual void setTitle(const std::string& title) = 0;
    virtual void setGenre(const std::string& genre) = 0;
    virtual void setDuration(int duration) = 0;
    virtual void setDescription(const std::string& description) = 0;
    virtual void setRating(float rating) = 0;

};
    
#endif //IMOVIE_H