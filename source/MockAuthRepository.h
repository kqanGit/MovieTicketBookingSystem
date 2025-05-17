#ifndef MOCK_AUTH_REPOSITORY_H
#define MOCK_AUTH_REPOSITORY_H

#include "repository/IAuthenticationRepository.h"
#include "AccountInformation.h"
#include <map>
#include <stdexcept>
#include <iostream>

// This repository is used for testing purposes
// It doesn't require a database connection and stores users in memory
class MockAuthRepository : public IAuthenticationRepository {
private:
    std::map<std::string, AccountInformation> users;

public:
    MockAuthRepository() {
        std::cout << "MockAuthRepository created" << std::endl;
        
        // Add some test users
        AccountInformation admin;
        admin.userName = "admin";
        admin.password = "admin123";
        admin.gmail = "admin@system.com";
        admin.phoneNumber = "0123456789";
        admin.role = "admin";
        
        AccountInformation user;
        user.userName = "user";
        user.password = "user123";
        user.gmail = "user@example.com";
        user.phoneNumber = "9876543210";
        user.role = "user";
        
        users[admin.userName] = admin;
        users[user.userName] = user;
    }
    
    void addUser(const AccountInformation& account) override {
        std::cout << "Adding user: " << account.userName << std::endl;
        
        // Check if user already exists
        if (users.find(account.userName) != users.end()) {
            throw std::runtime_error("User already exists");
        }
        
        // Add user
        users[account.userName] = account;
        std::cout << "User added successfully" << std::endl;
    }
    
    AccountInformation getUserByUserName(const std::string& username, const std::string& password) override {
        std::cout << "Getting user: " << username << std::endl;
        
        // Check if user exists
        auto it = users.find(username);
        if (it == users.end()) {
            throw std::runtime_error("User not found");
        }
        
        // Check password
        if (it->second.password != password) {
            throw std::runtime_error("Invalid password");
        }
        
        return it->second;
    }
};

#endif // MOCK_AUTH_REPOSITORY_H