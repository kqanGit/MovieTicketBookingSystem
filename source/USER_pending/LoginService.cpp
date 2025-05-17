// LoginService.cpp
#include "LoginService.h"
#include <iostream>

std::unique_ptr<IUserContext> LoginService::login(const std::string& username, const std::string& password) {
    try {
        AccountInformation info = repo->getUserByUserName(username, password);
        if (info.role == "admin") return adminFactory->CreateUser(info);
        else return userFactory->CreateUser(info);
    } catch(const std::exception& e) {
        std::cout << "Login failed: " << e.what() << std::endl;
        return nullptr;
    }
}