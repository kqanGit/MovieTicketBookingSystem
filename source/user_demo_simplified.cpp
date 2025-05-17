#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <vector>
#include "demo_app.h"

class App {
private:
    std::unique_ptr<UserManager> userManager;
    MockAuthRepository* authRepo;

public:
    App() : authRepo(nullptr) {}
    
    ~App() {
        if (authRepo) {
            delete authRepo;
            authRepo = nullptr;
        }
    }
    
    bool initialize() {
        std::cout << "Đang khởi tạo ứng dụng..." << std::endl;
        
        // Khởi tạo repository
        authRepo = new MockAuthRepository();
        
        // Khởi tạo UserManager với repository
        userManager = std::make_unique<UserManager>(authRepo);
        
        std::cout << "Khởi tạo hoàn tất." << std::endl;
        return true;
    }
    
    void run() {
        std::cout << "Chào mừng đến với hệ thống đặt vé xem phim!" << std::endl;
        
        bool running = true;
        while(running) {
            // Lấy context người dùng hiện tại
            auto currentUser = userManager->getCurrentUser();
            
            std::cout << "\n----- MENU CHÍNH -----" << std::endl;
            std::cout << "Vai trò hiện tại: " << currentUser->getRole() << std::endl;
            
            // Menu tùy theo vai trò
            if (currentUser->getRole() == "guest") {
                std::cout << "1. Đăng nhập" << std::endl;
                std::cout << "2. Đăng ký" << std::endl;
                std::cout << "3. Xem danh sách phim" << std::endl;
            } else if (currentUser->getRole() == "user") {
                std::cout << "1. Xem thông tin cá nhân" << std::endl;
                std::cout << "2. Xem danh sách phim" << std::endl;
                std::cout << "3. Đặt vé" << std::endl;
                std::cout << "4. Xem lịch sử đặt vé" << std::endl;
                std::cout << "5. Đăng xuất" << std::endl;
            } else if (currentUser->getRole() == "admin") {
                std::cout << "1. Quản lý phim" << std::endl;
                std::cout << "2. Xem danh sách phim" << std::endl;
                std::cout << "3. Đăng xuất" << std::endl;
            }
            
            std::cout << "0. Thoát" << std::endl;
            std::cout << "Lựa chọn của bạn: ";
            
            int choice;
            std::cin >> choice;
            
            // Xử lý menu cho GUEST
            if (currentUser->getRole() == "guest") {
                switch(choice) {
                    case 1: { // Đăng nhập
                        std::string username, password;
                        std::cout << "Tên đăng nhập (admin/user): ";
                        std::cin >> username;
                        std::cout << "Mật khẩu (admin123/user123): ";
                        std::cin >> password;
                        
                        userManager->login(username, password);
                        break;
                    }
                    case 2: { // Đăng ký
                        AccountInformation info;
                        std::cout << "Tên đăng nhập: ";
                        std::cin >> info.userName;
                        std::cout << "Mật khẩu: ";
                        std::cin >> info.password;
                        std::cout << "Email: ";
                        std::cin >> info.gmail;
                        std::cout << "Số điện thoại: ";
                        std::cin >> info.phoneNumber;
                        info.role = "user"; // Mặc định là user
                        
                        userManager->registerUser(info);
                        break;
                    }
                    case 3: { // Xem danh sách phim
                        std::cout << "Đang hiển thị danh sách phim..." << std::endl;
                        std::cout << "Chức năng này chưa được triển khai." << std::endl;
                        break;
                    }
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Lựa chọn không hợp lệ!" << std::endl;
                }
            }
            // Xử lý menu cho USER
            else if (currentUser->getRole() == "user") {
                switch(choice) {
                    case 1: // Xem thông tin cá nhân
                        std::cout << "Đang hiển thị thông tin cá nhân..." << std::endl;
                        std::cout << "Chức năng này chưa được triển khai." << std::endl;
                        break;
                    case 2: // Xem danh sách phim
                        std::cout << "Đang hiển thị danh sách phim..." << std::endl;
                        std::cout << "Chức năng này chưa được triển khai." << std::endl;
                        break;                    case 3: // Đặt vé
                        userManager->bookTicket();
                        break;
                    case 4: // Xem lịch sử đặt vé
                        userManager->viewBookingHistory();
                        break;
                    case 5: // Đăng xuất
                        userManager->logout();
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Lựa chọn không hợp lệ!" << std::endl;
                }
            }
            // Xử lý menu cho ADMIN
            else if (currentUser->getRole() == "admin") {
                switch(choice) {                    case 1: // Quản lý phim
                        userManager->manageMovies();
                        break;
                    case 2: // Xem danh sách phim
                        std::cout << "Đang hiển thị danh sách phim..." << std::endl;
                        std::cout << "Chức năng này chưa được triển khai." << std::endl;
                        break;
                    case 3: // Đăng xuất
                        userManager->logout();
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Lựa chọn không hợp lệ!" << std::endl;
                }
            }
        }
    }
    
    void shutdown() {
        std::cout << "Đang đóng ứng dụng..." << std::endl;
        std::cout << "Đã đóng ứng dụng. Tạm biệt!" << std::endl;
    }
};

int main() {
    App app;
    
    // Khởi tạo ứng dụng
    if (!app.initialize()) {
        std::cerr << "Không thể khởi tạo ứng dụng. Đang thoát..." << std::endl;
        return 1;
    }
    
    // Chạy ứng dụng
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Lỗi không xử lý được: " << e.what() << std::endl;
        return 2;
    }
    
    // Đóng ứng dụng
    app.shutdown();
    
    return 0;
}
