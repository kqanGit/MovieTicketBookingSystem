#include "SessionManager.h"
#include "model/Guest.h" 
#include "model/User.h" 
#include "model/Admin.h" 
#include "context/GuestContextCreator.h"
#include "context/UserContextCreator.h"
#include "context/AdminContextCreator.h"
#include <iostream>
#include <stdexcept>

SessionManager::SessionManager() : _isAuthenticated(false) {
    // Khởi tạo factory mặc định là GuestContextCreator
    _contextFactory = std::make_shared<GuestContextCreator>();
    
    // Khởi tạo với Guest context
    _currentUserContext = _contextFactory->CreateUser();
    
    std::cout << "Session initiated with Guest context" << std::endl;
}

IUserContext* SessionManager::getCurrentContext() const {
    return _currentUserContext.get();
}

std::string SessionManager::getCurrentRole() const {
    if (!_currentUserContext) {
        return "unknown";
    }
    
    // Dựa vào loại context để xác định vai trò
    auto guest = dynamic_cast<Guest*>(_currentUserContext.get());
    if (guest) return "Guest";
    
    auto user = dynamic_cast<User*>(_currentUserContext.get());
    if (user) return "User";
    
    auto admin = dynamic_cast<Admin*>(_currentUserContext.get());
    if (admin) return "Admin";
    
    return "unknown";
}

bool SessionManager::isUserAuthenticated() const {
    return _isAuthenticated;
}

const AccountInformation& SessionManager::getCurrentAccount() const {
    return _currentAccount;
}

bool SessionManager::setUserContext(const AccountInformation& authInfo) {
    if (_isAuthenticated) {
        std::cout << "Login as role: " << getCurrentRole() << std::endl;
        return false;
    }

    // Cập nhật factory dựa trên vai trò
    if (authInfo.role == "Admin") {
        _contextFactory = std::make_shared<AdminContextCreator>();
        _currentUserContext = _contextFactory->CreateUser(authInfo);
    } else if (authInfo.role == "User") {
        _contextFactory = std::make_shared<UserContextCreator>();
        _currentUserContext = _contextFactory->CreateUser(authInfo);
    } else if (authInfo.role == "Guest") {
        _contextFactory = std::make_shared<GuestContextCreator>();
        _currentUserContext = _contextFactory->CreateUser();
    } else {
        std::cout << "Invalid role: " << authInfo.role << std::endl;
        return false;
    }

    _currentAccount = authInfo;
    _isAuthenticated = (authInfo.role != "Guest");
    std::cout << "[Session] Already Init context: " << getCurrentRole() << std::endl;
    return true;
}

bool SessionManager::logout(std::shared_ptr<ILogoutService> service) {
   if (!_isAuthenticated) {
        std::cout << "[Session] Not Login yet" << std::endl;
        return false;
    }

    if (!service) {
        std::cerr << "[Session] LogoutService is null.\n";
        return false;
    }

    _currentUserContext = service->logout();
    _contextFactory = std::make_shared<GuestContextCreator>();
    _isAuthenticated = false;
    _currentAccount = AccountInformation();

    std::cout << "[Session] Logout successfully" << std::endl;
    return true;
}