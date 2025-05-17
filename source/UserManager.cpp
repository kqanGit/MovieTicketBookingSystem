#include "UserManager.h"
#include <iostream>

UserManager::UserManager(IAuthenticationRepository* repo) : authRepo(repo) {
    // Khởi tạo với người dùng mặc định là Guest
    currentUser = std::make_unique<Guest>(authRepo);
    std::cout << "Khởi tạo người dùng mặc định: Guest" << std::endl;
}

IUserContext* UserManager::getCurrentUser() const {
    return currentUser.get();
}

bool UserManager::login(const std::string& username, const std::string& password) {
    // Kiểm tra xem có phải là Guest không
    if (currentUser->getRole() != "guest") {
        std::cout << "Bạn đã đăng nhập với vai trò: " << currentUser->getRole() << std::endl;
        return false;
    }

    // Lấy service đăng nhập từ context hiện tại
    auto loginService = currentUser->getLoginService();
    if (!loginService) {
        std::cout << "Không thể thực hiện đăng nhập!" << std::endl;
        return false;
    }

    try {
        // Thực hiện đăng nhập và lấy context mới
        auto newContext = loginService->login(username, password);
        if (newContext) {
            // Chuyển sang context mới (User hoặc Admin)
            currentUser = std::move(newContext);
            std::cout << "Đăng nhập thành công với vai trò: " << currentUser->getRole() << std::endl;
            return true;
        } else {
            std::cout << "Đăng nhập thất bại: Thông tin không hợp lệ" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "Lỗi đăng nhập: " << e.what() << std::endl;
        return false;
    }
}

bool UserManager::logout() {
    // Kiểm tra xem có phải là Guest không
    if (currentUser->getRole() == "guest") {
        std::cout << "Bạn chưa đăng nhập!" << std::endl;
        return false;
    }    // Lấy service đăng xuất từ context hiện tại
    auto logoutService = currentUser->getLogoutService();
    if (!logoutService) {
        std::cout << "Không thể thực hiện đăng xuất!" << std::endl;
        return false;
    }
    
    // Thực hiện đăng xuất và lấy context mới (Guest)
    currentUser = std::move(logoutService->logout());
    std::cout << "Đăng xuất thành công" << std::endl;
    return true;
}

bool UserManager::registerUser(const AccountInformation& info) {
    // Chỉ Guest mới có thể đăng ký
    if (currentUser->getRole() != "guest") {
        std::cout << "Vui lòng đăng xuất trước khi đăng ký tài khoản mới" << std::endl;
        return false;
    }

    // Lấy service đăng ký từ context hiện tại
    auto registerService = currentUser->getRegisterService();
    if (!registerService) {
        std::cout << "Không thể thực hiện đăng ký!" << std::endl;
        return false;
    }    try {
        // Thực hiện đăng ký
        registerService->addUser(info);
        std::cout << "Đăng ký thành công với tài khoản: " << info.userName << std::endl;
        
        // Sau khi đăng ký, tự động đăng nhập
        return login(info.userName, info.password);
    } catch (const std::exception& e) {
        std::cout << "Lỗi đăng ký: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "Lỗi đăng ký: " << e.what() << std::endl;
        return false;
    }
}
