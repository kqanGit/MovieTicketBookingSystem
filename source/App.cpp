#include "App.h"
#include <iostream>
#include <string>
#include "USER_pending/MovieManagerService.h"
#include "USER_pending/BookingService.h"
#include "USER_pending/ViewBookingHistoryService.h"
#include "USER_pending/MovieViewerService.h"
#include "USER_pending/SessionManager.h"

App::App(bool useMock) : dbConn(nullptr), authRepo(nullptr), useMockRepo(useMock) {
}

App::~App() {
    if (authRepo) {
        delete authRepo;
        authRepo = nullptr;
    }
    // dbConn là singleton nên không cần delete
}

bool App::initialize() {
    std::cout << "Đang khởi tạo ứng dụng..." << std::endl;
    
    if (useMockRepo) {
        // Use mock repository for testing
        std::cout << "Using mock repository for testing" << std::endl;
        authRepo = new MockAuthRepository();
    } else {
        // Initialize database connection
        dbConn = DatabaseConnection::getInstance();
        if (!dbConn->connect("database.db")) {
            std::cerr << "Không thể kết nối tới cơ sở dữ liệu." << std::endl;
            return false;
        }
        
        // Run database script if needed
        dbConn->executeSQLFile("./database/database.sql");
        
        // Initialize SQL repository
        authRepo = new AuthenticationRepositorySQL(dbConn);
    }
      // Initialize SessionManager with repository
    sessionManager = std::make_unique<SessionManager>(authRepo);
    
    std::cout << "Khởi tạo hoàn tất." << std::endl;
    return true;
}

void App::run() {
    std::cout << "Chào mừng đến với hệ thống đặt vé xem phim!" << std::endl;
    
    bool running = true;    while(running) {
        // Lấy context người dùng hiện tại
        auto currentUser = sessionManager->getCurrentContext();
        
        std::cout << "\n----- MENU CHÍNH -----" << std::endl;
        std::cout << "Vai trò hiện tại: " << sessionManager->getCurrentRole() << std::endl;
        
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
                case 1: { // Đăng nhập                    std::string username, password;
                    std::cout << "Tên đăng nhập (admin/admin123, user/user123): ";
                    std::cin >> username;
                    std::cout << "Mật khẩu: ";
                    std::cin >> password;
                    
                    sessionManager->login(username, password);
                    break;
                }
                case 2: { // Đăng ký
                    AccountInformation info;
                    std::cout << "Tên đăng nhập: ";
                    std::cin >> info.userName;
                    std::cout << "Mật khẩu: ";
                    std::cin >> info.password;
                    std::cout << "Email: ";
                    std::cin >> info.gmail;                    std::cout << "Số điện thoại: ";
                    std::cin >> info.phoneNumber;
                    info.role = "user"; // Mặc định là user
                    
                    sessionManager->registerUser(info);
                    break;
                }
                case 3: { // Xem danh sách phim
                    testViewMovie();
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
                    std::cout << "Chức năng này chưa được triển khai đầy đủ." << std::endl;
                    break;
                case 2: // Xem danh sách phim
                    testViewMovie();
                    break;
                case 3: // Đặt vé
                    testBookingFunctionality();
                    break;
                case 4: // Xem lịch sử đặt vé
                    testHistoryViewing();
                    break;                case 5: // Đăng xuất
                    sessionManager->logout();
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
                    testMovieManagement();
                    break;
                case 2: // Xem danh sách phim
                    testViewMovie();
                    break;                case 3: // Đăng xuất
                    sessionManager->logout();
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

void App::shutdown() {
    std::cout << "Đang đóng ứng dụng..." << std::endl;
    
    // Đóng kết nối database
    if (dbConn) {
        dbConn->disconnect();
    }
    
    std::cout << "Đã đóng ứng dụng. Tạm biệt!" << std::endl;
}

void App::testMovieManagement() {
    std::cout << "\n=== Testing Movie Management (Admin) ===" << std::endl;
    
    auto currentUser = userManager->getCurrentUser();
    if (currentUser->getRole() != "admin") {
        std::cout << "This functionality is only available for admin users" << std::endl;
        return;
    }
    
    auto movieManager = currentUser->getMovieManagerService();
    if (movieManager) {
        // Attempt to dynamic_cast to access concrete methods
        auto* manager = dynamic_cast<MovieManagerService*>(movieManager);
        if (manager) {
            manager->addMovie();
            manager->updateMovie();
            manager->deleteMovie();
            manager->listMovies();
        } else {
            std::cout << "Could not access MovieManagerService methods" << std::endl;
        }
    } else {
        std::cout << "Movie manager service is not available" << std::endl;
    }
}

void App::testBookingFunctionality() {
    std::cout << "\n=== Testing Booking Functionality (User) ===" << std::endl;
    
    auto currentUser = userManager->getCurrentUser();
    if (currentUser->getRole() != "user" && currentUser->getRole() != "admin") {
        std::cout << "Please log in to access booking functionality" << std::endl;
        return;
    }
    
    auto bookingService = currentUser->getBookingService();
    if (bookingService) {
        // Attempt to dynamic_cast to access concrete methods
        auto* booking = dynamic_cast<BookingService*>(bookingService);
        if (booking) {
            booking->bookTicket();
            booking->cancelBooking();
            booking->viewShowtimes();
            booking->viewAvailableSeats();
        } else {
            std::cout << "Could not access BookingService methods" << std::endl;
        }
    } else {
        std::cout << "Booking service is not available" << std::endl;
    }
}

void App::testHistoryViewing() {
    std::cout << "\n=== Testing Booking History Viewing (User) ===" << std::endl;
    
    auto currentUser = userManager->getCurrentUser();
    if (currentUser->getRole() != "user" && currentUser->getRole() != "admin") {
        std::cout << "Please log in to access booking history" << std::endl;
        return;
    }
    
    auto historyService = currentUser->getViewBookingHistoryService();
    if (historyService) {
        // Attempt to dynamic_cast to access concrete methods
        auto* history = dynamic_cast<ViewBookingHistoryService*>(historyService);
        if (history) {
            history->viewAllBookings();
            history->viewBookingDetails();
        } else {
            std::cout << "Could not access ViewBookingHistoryService methods" << std::endl;
        }
    } else {
        std::cout << "History viewing service is not available" << std::endl;
    }
}

void App::testViewMovie() {
    std::cout << "\n=== Testing Movie Viewing Functionality ===" << std::endl;
    
    auto currentUser = userManager->getCurrentUser();
    auto movieViewer = currentUser->getMovieViewerService();
    
    if (movieViewer) {
        // Attempt to dynamic_cast to access concrete methods
        auto* viewer = dynamic_cast<MovieViewerService*>(movieViewer);
        if (viewer) {
            viewer->viewMovieList();
            viewer->viewMovieDetails(1);
            viewer->searchMovies("action");
        } else {
            std::cout << "Could not access MovieViewerService methods" << std::endl;
        }
    } else {
        std::cout << "Movie viewing service is not available" << std::endl;
    }
}