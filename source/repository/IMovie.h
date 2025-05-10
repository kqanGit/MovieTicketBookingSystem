#ifndef IMOVIE_H
#define IMOVIE_H

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class IMovie {
    public:
        virtual ~IMovie() {}
        virtual int getId() const = 0;
        virtual string getTitle() const = 0;
        virtual string getGenre() const = 0;
        virtual int getDuration() const = 0;
        virtual string getDescription() const = 0;
        virtual float getRating() const = 0;
};
    
#endif //IMOVIE_H