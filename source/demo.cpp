#include <iostream>
#include <memory>
#include <string>

// Định nghĩa các interface
class IUserContext {
public:
    virtual std::string getRole() const = 0;
    virtual ~IUserContext() = default;
};

// Các lớp Context
class Guest : public IUserContext {
public:
    std::string getRole() const override { return "guest"; }
};

class User : public IUserContext {
private:
    std::string username;
public:
    User(const std::string& name) : username(name) {}
    std::string getRole() const override { return "user"; }
    std::string getUsername() const { return username; }
};

class Admin : public IUserContext {
private:
    std::string username;
public:
    Admin(const std::string& name) : username(name) {}
    std::string getRole() const override { return "admin"; }
    std::string getUsername() const { return username; }
};

// Quản lý User
class UserManager {
private:
    std::unique_ptr<IUserContext> currentUser;

public:
    UserManager() : currentUser(std::make_unique<Guest>()) {
        std::cout << "Khởi tạo với người dùng mặc định: " << currentUser->getRole() << std::endl;
    }

    bool login(const std::string& username, const std::string& password) {
        if (currentUser->getRole() != "guest") {
            std::cout << "Bạn đã đăng nhập rồi!" << std::endl;
            return false;
        }

        // Giả lập đăng nhập
        if (username == "admin" && password == "admin123") {
            currentUser = std::make_unique<Admin>(username);
            std::cout << "Đăng nhập thành công với vai trò: " << currentUser->getRole() << std::endl;
            return true;
        } else if (username == "user" && password == "user123") {
            currentUser = std::make_unique<User>(username);
            std::cout << "Đăng nhập thành công với vai trò: " << currentUser->getRole() << std::endl;
            return true;
        }

        std::cout << "Đăng nhập thất bại: Thông tin không hợp lệ" << std::endl;
        return false;
    }

    bool logout() {
        if (currentUser->getRole() == "guest") {
            std::cout << "Bạn chưa đăng nhập!" << std::endl;
            return false;
        }

        currentUser = std::make_unique<Guest>();
        std::cout << "Đã đăng xuất thành công" << std::endl;
        return true;
    }

    IUserContext* getCurrentUser() const {
        return currentUser.get();
    }
};

// Class App quản lý ứng dụng
class App {
private:
    UserManager userManager;

public:
    void run() {
        std::cout << "Chào mừng đến với hệ thống đặt vé xem phim!" << std::endl;
        
        bool running = true;
        while(running) {
            auto currentUser = userManager.getCurrentUser();
            
            std::cout << "\n----- MENU CHÍNH -----" << std::endl;
            std::cout << "Vai trò hiện tại: " << currentUser->getRole() << std::endl;
            std::cout << "1. Đăng nhập" << std::endl;
            std::cout << "2. Đăng xuất" << std::endl;
            std::cout << "0. Thoát" << std::endl;
            std::cout << "Lựa chọn của bạn: ";
            
            int choice;
            std::cin >> choice;
            
            switch(choice) {
                case 1: { // Đăng nhập
                    std::string username, password;
                    std::cout << "Tên đăng nhập (admin/user): ";
                    std::cin >> username;
                    std::cout << "Mật khẩu (admin123/user123): ";
                    std::cin >> password;
                    
                    userManager.login(username, password);
                    break;
                }
                case 2: // Đăng xuất
                    userManager.logout();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Lựa chọn không hợp lệ!" << std::endl;
            }
        }

        std::cout << "Cảm ơn bạn đã sử dụng ứng dụng!" << std::endl;
    }
};

// Định nghĩa hàm main
int main() {
    App app;
    app.run();
    return 0;
}
