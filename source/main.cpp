#include <iostream>
#include "App.h"

int main() {
    std::cout << "=== Movie Ticket Booking System ===" << std::endl;
    std::cout << "\nThe system will start in Guest mode.\n";
    std::cout << "You can log in, register, or browse movies as a Guest." << std::endl;
    std::cout << "After successful login, you will switch to User or Admin mode. Logging out will return you to Guest mode." << std::endl;
    std::cout << "\nDatabase: source/database.db (SQLite)\n";

    // Khởi tạo App với repository thực (kết nối database)
    App app(false); // false = dùng repository thật, true = mock
    if (!app.initialize()) {
        std::cerr << "[App] Unable to initialize the application. Exiting..." << std::endl;
        return 1;
    }

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "[Main]Failed to process: " << e.what() << std::endl;
        return 2;
    }

    app.shutdown();
    return 0;
}