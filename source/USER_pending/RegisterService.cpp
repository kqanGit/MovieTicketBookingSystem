#include "RegisterService.h"
#include <iostream>

std::unique_ptr<IUserContext> RegisterService::registerAccount(const AccountInformation& info) {
    try {
        repo->addUser(info);
        if (factory) {
            return factory->CreateUser(info);
        }
        return nullptr;
    } catch (const std::exception& e) {
        std::cout << "Registration failed: " << e.what() << std::endl;
        return nullptr;
    }
}
