#include "RegisterService.h"
#include <iostream>

bool RegisterService::registerUser(const AccountInformation& info) {
    try {
        if (repo) {
            repo->addUser(info);
            std::cout << "Đăng ký thành công: " << info.userName << std::endl;
            return true;
        }
        std::cout << "Không thể đăng ký: Repository không khả dụng" << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "Đăng ký thất bại: " << e.what() << std::endl;
        return false;
    }
}
