#include "UserInformationService.h"
#include <iostream>

UserInformationService::UserInformationService(const AccountInformation& acc)
    : accountInfo(acc) {}

void UserInformationService::printAccountInfo() const {
    std::cout << "--- Account Information ---\n";
    std::cout << "Username: " << accountInfo.userName << '\n';
    std::cout << "Password: " << accountInfo.password << '\n';
    std::cout << "Email: " << accountInfo.gmail << '\n';
    std::cout << "Phone: " << accountInfo.phoneNumber << '\n';
    std::cout << "Role: " << accountInfo.role << '\n';
}
