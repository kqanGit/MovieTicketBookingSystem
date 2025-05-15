// LoginService.cpp
#include "LoginService.h"

std::unique_ptr<IUserContext> LoginService::login(const std::string& username, const std::string& password) {
    auto infoOpt = repo->getUserByUserName(username, password);
    if (!infoOpt.has_value()) return nullptr;
    AccountInformation info = infoOpt.value();
    if (info.role == "admin") return adminFactory->CreateUser(info);
    else return userFactory->CreateUser(info);
}