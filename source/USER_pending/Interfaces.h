// Interfaces.h
#ifndef INTERFACES_H
#define INTERFACES_H
#include <string>
#include <optional>
#include <memory>
#include "AccountInformation.h"

// Forward declaration
class IUserContext;

// Service interfaces
class IUserInformationService { 
public: 
    virtual ~IUserInformationService() = default; 
};

class IMovieViewerService { 
public: 
    virtual void viewMovieList() = 0;
    virtual void viewMovieDetails(int movieId) = 0;
    virtual void searchMovies(const std::string& query) = 0;
    virtual ~IMovieViewerService() = default; 
};

class IMovieManagerService { 
public: 
    virtual void addMovie() = 0;
    virtual void updateMovie() = 0;
    virtual void deleteMovie() = 0;
    virtual void listMovies() = 0;
    virtual ~IMovieManagerService() = default; 
};

class IBookingService { 
public: 
    virtual void bookTicket() = 0;
    virtual void cancelBooking() = 0;
    virtual void viewShowtimes() = 0;
    virtual void viewAvailableSeats() = 0;
    virtual ~IBookingService() = default; 
}; 

class IViewBookingHistoryService { 
public: 
    virtual void viewAllBookings() = 0;
    virtual void viewBookingDetails() = 0;
    virtual ~IViewBookingHistoryService() = default; 
};

class ILoginService { 
public: 
    virtual std::unique_ptr<IUserContext> login(const std::string& username, const std::string& password) = 0; 
    virtual ~ILoginService() = default; 
};

class ILogoutService { 
public: 
    virtual std::unique_ptr<IUserContext> logout() = 0; 
    virtual ~ILogoutService() = default; 
};

class IRegisterService { 
public: 
    virtual std::unique_ptr<IUserContext> registerAccount(const AccountInformation& info) = 0; 
    virtual void addUser(const AccountInformation& info) = 0;
    virtual ~IRegisterService() = default; 
};
#endif