#include "SessionManager.h"
#include "Guest.h"
#include "User.h"
#include "Admin.h"
#include "GuestContextCreator.h"
#include "UserContextCreator.h"
#include "AdminContextCreator.h"
#include <iostream>
#include <stdexcept>

SessionManager::SessionManager(IAuthenticationRepository* repo) : 
    authRepo(repo), isAuthenticated(false) {
    // Initialize the factories
    guestFactory = std::make_unique<GuestContextCreator>(repo);
    userFactory = std::make_unique<UserContextCreator>(repo);
    adminFactory = std::make_unique<AdminContextCreator>(repo);
    
    // Start with a Guest context
    AccountInformation guestInfo;
    guestInfo.userName = "guest";
    guestInfo.role = "guest";
    currentContext = guestFactory->CreateUser(guestInfo);
    
    std::cout << "Session initialized with Guest context" << std::endl;
}

IUserContext* SessionManager::getCurrentContext() const {
    return currentContext.get();
}

std::string SessionManager::getCurrentRole() const {
    return currentContext->getRole();
}

bool SessionManager::isUserAuthenticated() const {
    return isAuthenticated;
}

const AccountInformation& SessionManager::getCurrentAccount() const {
    return currentAccount;
}

bool SessionManager::login(const std::string& username, const std::string& password) {
    // Check if already logged in
    if (isAuthenticated) {
        std::cout << "Already logged in as " << currentAccount.userName << " with role: " << currentContext->getRole() << std::endl;
        return false;
    }
    
    try {
        // Authenticate using repository
        AccountInformation info = authRepo->getUserByUserName(username, password);
        
        // Create appropriate context based on role
        if (info.role == "admin") {
            currentContext = adminFactory->CreateUser(info);
        } else {
            currentContext = userFactory->CreateUser(info);
        }
        
        // Update session state
        currentAccount = info;
        isAuthenticated = true;
        
        std::cout << "Login successful. Role: " << currentContext->getRole() << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Login failed: " << e.what() << std::endl;
        return false;
    }
}

bool SessionManager::logout() {
    // Check if logged in
    if (!isAuthenticated) {
        std::cout << "Not logged in" << std::endl;
        return false;
    }
    
    // Create guest context
    AccountInformation guestInfo;
    guestInfo.userName = "guest";
    guestInfo.role = "guest";
    currentContext = guestFactory->CreateUser(guestInfo);
    
    // Update session state
    isAuthenticated = false;
    currentAccount = AccountInformation(); // Clear account info
    
    std::cout << "Logout successful" << std::endl;
    return true;
}

bool SessionManager::registerUser(const AccountInformation& info) {
    try {
        // Add user to repository
        authRepo->addUser(info);
        std::cout << "User registered successfully: " << info.userName << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Registration failed: " << e.what() << std::endl;
        return false;
    }
}
