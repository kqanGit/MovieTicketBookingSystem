#include "SessionManager.h"
#include "Guest.h"
#include "User.h"
#include "Admin.h"
#include "GuestContextCreator.h"
#include "UserContextCreator.h"
#include "AdminContextCreator.h"
#include <iostream>
#include <stdexcept>

SessionManager::SessionManager() : isAuthenticated(false) {
    // Initialize the factories
    guestFactory = std::make_unique<GuestContextCreator>();
    userFactory = std::make_unique<UserContextCreator>();
    adminFactory = std::make_unique<AdminContextCreator>();
    
    // Start with a Guest context
    AccountInformation guestInfo;
    guestInfo.userName = "guest";
    guestInfo.role = "guest";
    currentContext = guestFactory->CreateUser(guestInfo);
    currentAccount = AccountInformation(); // Set default currentAccount for Guest
    
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

bool SessionManager::setUserContext(const AccountInformation& authInfo) {
    if (isAuthenticated) {
        std::cout << "Already logged in as " << currentAccount.userName << " with role: " << currentContext->getRole() << std::endl;
        return false;
    }

    if (authInfo.role == "admin") {
        currentContext = adminFactory->CreateUser(authInfo);
    } else if (authInfo.role == "user") {
        currentContext = userFactory->CreateUser(authInfo);
    } else if (authInfo.role == "guest") {
        currentContext = guestFactory->CreateUser();
    } else {
        std::cout << "Invalid role: " << authInfo.role << std::endl;
        return false;
    }

    currentAccount = authInfo;
    isAuthenticated = (authInfo.role != "guest");
    std::cout << "Context set to: " << currentContext->getRole() << std::endl;
    return true;
}

bool SessionManager::logout() {
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
    currentAccount = AccountInformation(); // Reset to default when Guest
    
    std::cout << "Logout successful" << std::endl;
    return true;
}
