#include <iostream>
#include "App.h"

int main() {
    std::cout << "=== Movie Ticket Booking System ===" << std::endl;
    std::cout << "\nHệ thống sẽ bắt đầu với vai trò Guest.\n";
    std::cout << "Bạn có thể đăng nhập (login), đăng ký (register), hoặc xem phim với vai trò Guest." << std::endl;
    std::cout << "Sau khi đăng nhập thành công sẽ chuyển sang User hoặc Admin. Đăng xuất sẽ trở lại Guest." << std::endl;
    std::cout << "\nDatabase: source/database.db (SQLite)\n";

    // Khởi tạo App với repository thực (kết nối database)
    App app(false); // false = dùng repository thật, true = mock
    if (!app.initialize()) {
        std::cerr << "Không thể khởi tạo ứng dụng. Đang thoát..." << std::endl;
        return 1;
    }

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Lỗi không xử lý được: " << e.what() << std::endl;
        return 2;
    }

    app.shutdown();
    return 0;
}