#include <iostream>
#include <memory>
#include <string>

// Include các file từ USER_pending
#include "../USER_pending/AccountInformation.h"
#include "../USER_pending/IUserContext.h"
#include "../USER_pending/Guest.h"
#include "../USER_pending/User.h"
#include "../USER_pending/Admin.h"
#include "../USER_pending/LoginService.h"
#include "../USER_pending/LogoutService.h"
#include "../USER_pending/RegisterService.h"
#include "repository/AuthenticationRepositorySQL.h"
#include "repository/IAuthenticationRepository.h"
#include "database/DatabaseConnection.h"

// Class UserManager quản lý người dùng
class UserManager {
private:
    std::unique_ptr<IUserContext> currentUser;
    IAuthenticationRepository* authRepo;

public:
    UserManager(IAuthenticationRepository* repo) : authRepo(repo) {
        // Khởi tạo với người dùng mặc định là Guest
        currentUser = std::make_unique<Guest>(authRepo);
        std::cout << "Khởi tạo người dùng mặc định: " << currentUser->getRole() << std::endl;
    }

    IUserContext* getCurrentUser() const {
        return currentUser.get();
    }

    bool login(const std::string& username, const std::string& password) {
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

    bool logout() {
        // Kiểm tra xem có phải là Guest không
        if (currentUser->getRole() == "guest") {
            std::cout << "Bạn chưa đăng nhập!" << std::endl;
            return false;
        }

        // Lấy service đăng xuất từ context hiện tại
        auto logoutService = currentUser->getLogoutService();
        if (!logoutService) {
            std::cout << "Không thể thực hiện đăng xuất!" << std::endl;
            return false;
        }

        // Thực hiện đăng xuất và lấy context mới (Guest)
        currentUser = logoutService->logout();
        std::cout << "Đăng xuất thành công" << std::endl;
        return true;
    }

    bool registerUser(const AccountInformation& info) {
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
        }

        try {
            // Thực hiện đăng ký
            auto newContext = registerService->registerAccount(info);
            if (newContext) {
                // Chuyển sang context mới (User)
                currentUser = std::move(newContext);
                std::cout << "Đăng ký và đăng nhập thành công với tài khoản: " << info.userName << std::endl;
                return true;
            } else {
                std::cout << "Đăng ký thất bại" << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "Lỗi đăng ký: " << e.what() << std::endl;
            return false;
        }
    }
};

// Class App quản lý ứng dụng
class App {
private:
    std::unique_ptr<UserManager> userManager;
    DatabaseConnection* dbConn;
    AuthenticationRepositorySQL* authRepo;

public:
    App() : dbConn(nullptr), authRepo(nullptr) {}
    
    ~App() {
        if (authRepo) {
            delete authRepo;
            authRepo = nullptr;
        }
        // dbConn là singleton nên không cần delete
    }
    
    bool initialize() {
        std::cout << "Đang khởi tạo ứng dụng..." << std::endl;
        
        // Khởi tạo kết nối database
        dbConn = DatabaseConnection::getInstance();
        if (!dbConn->connect("database.db")) {
            std::cerr << "Không thể kết nối tới cơ sở dữ liệu." << std::endl;
            return false;
        }
        
        // Chạy script tạo database nếu cần
        dbConn->executeSQLFile("./database/database.sql");
        
        // Khởi tạo repository
        authRepo = new AuthenticationRepositorySQL(dbConn);
        
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
                        std::cout << "Tên đăng nhập: ";
                        std::cin >> username;
                        std::cout << "Mật khẩu: ";
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
                        // TODO: Implement movie viewing
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
                        // TODO: Implement user info viewing
                        break;
                    case 2: // Xem danh sách phim
                        std::cout << "Đang hiển thị danh sách phim..." << std::endl;
                        // TODO: Implement movie viewing
                        break;
                    case 3: // Đặt vé
                        std::cout << "Đang vào chức năng đặt vé..." << std::endl;
                        // TODO: Implement booking
                        break;
                    case 4: // Xem lịch sử đặt vé
                        std::cout << "Đang hiển thị lịch sử đặt vé..." << std::endl;
                        // TODO: Implement booking history
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
                switch(choice) {
                    case 1: // Quản lý phim
                        std::cout << "Đang vào chức năng quản lý phim..." << std::endl;
                        // TODO: Implement movie management
                        break;
                    case 2: // Xem danh sách phim
                        std::cout << "Đang hiển thị danh sách phim..." << std::endl;
                        // TODO: Implement movie viewing
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
        
        // Đóng kết nối database
        if (dbConn) {
            dbConn->disconnect();
        }
        
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