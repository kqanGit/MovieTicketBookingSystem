#include "RegisterService.h"
#include <iostream>

bool RegisterService::registerUser(const AccountInformation& info) {
    try {
        if (repo) {
            repo->addUser(info);
            std::cout << "[RegisterService] Registerd Successfully: " << info.userName << std::endl;
            return true;
        }
        std::cout << "[RegisterService] Unable to Register: Repository Unavailable" << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "[RegisterService] Failed to register: " << e.what() << std::endl;
        return false;
    }
}
