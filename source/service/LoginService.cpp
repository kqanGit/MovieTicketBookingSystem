// LoginService.cpp
#include "LoginService.h"
#include <iostream>
#include <optional>

std::optional<AccountInformation> LoginService::authenticate(const std::string& username, const std::string& password) {
    try {
        AccountInformation info = repo->getUserByUserName(username, password);
        return info;
    } catch(const std::exception& e) {
        std::cout << "[LoginService] Login failed: " << e.what() << std::endl;
        return std::nullopt;
    }
}