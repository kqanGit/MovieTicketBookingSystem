extern "C" {
    #include "sqlite3.h"
}
#include "DatabaseConnection.h"
#include <iostream>

int main() {
    // Bước 1: Lấy thể hiện Singleton
    DatabaseConnection* db = DatabaseConnection::getInstance();

    // Bước 2: Kết nối đến file database (SQLite sẽ tạo mới nếu chưa tồn tại)
    if (!db->connect("database.db")) {
        std::cerr << "Không thể kết nối tới cơ sở dữ liệu.\n";
        return 1;
    }

    // // (Tùy chọn) Bước 3: Chạy schema.sql để tạo bảng và chèn dữ liệu mẫu nếu file db mới
    db->executeSQLFile("./database/database.sql");

    // // Bước 4: Truy vấn dữ liệu
    // std::string sql = "SELECT * FROM MOVIE";
    // auto results = db->executeQuery(sql);

    // // Bước 5: In kết quả
    // std::cout << "🎬 Danh sách phim:\n";
    // for (const auto& row : results) {
    //     std::cout << "🎬 MovieID: " << row.at("MovieID")
    //               << " | Title: " << row.at("Title")
    //               << " | Genre: " << row.at("Genre")
    //               << " | Description: " << row.at("Descriptions")
    //               << " | Rating: " << row.at("Rating") << '\n';
    // }

    // Bước 6: Đóng kết nối
    db->disconnect();
    return 0;
}