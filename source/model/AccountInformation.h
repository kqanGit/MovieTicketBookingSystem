// AccountInformation.h
#ifndef ACCOUNT_INFORMATION_H
#define ACCOUNT_INFORMATION_H

#include <string>

struct AccountInformation {
    int userID; // Added userID
    std::string userName;
    std::string password;
    std::string phoneNumber;
    std::string gmail;
    std::string role;

    // Default constructor to initialize userID, for example
    AccountInformation() : userID(0) {}
};

#endif // ACCOUNT_INFORMATION_H