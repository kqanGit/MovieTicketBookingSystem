#include <iostream>
#include <cassert>
#include "../repository/IBookingRepository.h"
#include "../repository/BookingRepositorySQL.h"
#include "../database/DatabaseConnection.h"
#include <memory>
#include <vector>

// Hàm chuẩn bị dữ liệu test
void setupTestData(DatabaseConnection* db) {
    std::cout << "Setting up test data..." << std::endl;
    
    // Xóa dữ liệu cũ để đảm bảo test sạch
    db->executeNonQuery("DELETE FROM BOOKSEAT", {});
    db->executeNonQuery("DELETE FROM BOOKING", {});
    db->executeNonQuery("DELETE FROM SHOWTIME", {});
    db->executeNonQuery("DELETE FROM MOVIE", {});
    db->executeNonQuery("DELETE FROM SEAT", {});
    db->executeNonQuery("DELETE FROM SEATTYPE", {});
    db->executeNonQuery("DELETE FROM ACCOUNT", {});
    
    // Thêm dữ liệu test
    db->executeNonQuery("INSERT INTO ACCOUNT (UserID, Password, RoleUser, Gmail, PhoneNumber, UserName) VALUES (1, 'password', 'User', 'test@example.com', '0912345678', 'John Doe')", {});
    
    db->executeNonQuery("INSERT INTO SEATTYPE (SeatType, Price) VALUES ('Single', 50.0)", {});
    db->executeNonQuery("INSERT INTO SEATTYPE (SeatType, Price) VALUES ('Couple', 90.0)", {});
    
    db->executeNonQuery("INSERT INTO SEAT (SeatID, SeatType, Price) VALUES ('A1', 'Single', 50.0)", {});
    db->executeNonQuery("INSERT INTO SEAT (SeatID, SeatType, Price) VALUES ('A2', 'Single', 50.0)", {});
    db->executeNonQuery("INSERT INTO SEAT (SeatID, SeatType, Price) VALUES ('B1', 'Couple', 90.0)", {});
    
    db->executeNonQuery("INSERT INTO MOVIE (MovieID, Title, Genre, Descriptions, Rating) VALUES (1, 'Movie A', 'Action', 'Test movie', 8.5)", {});
    
    db->executeNonQuery("INSERT INTO SHOWTIME (ShowTimeID, MovieID, Date, StartTime, EndTime) VALUES (1, 1, '2025-05-11', '10:00', '12:00')", {});
}

void testAddBooking() {
    BookingRepository repository;
    
    // Thêm booking mới
    try {
        repository.addBooking(1, 1); // userID 1, showTimeID 1
        std::cout << "Added booking successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error adding booking: " << e.what() << std::endl;
        assert(false); // Test thất bại
        return;
    }
    
    // Kiểm tra xem booking đã được thêm chưa
    auto db = DatabaseConnection::getInstance();
    auto result = db->executeQuery("SELECT COUNT(*) as count FROM BOOKING WHERE UserID = 1 AND ShowTimeID = 1", {});
    assert(!result.empty());
    assert(std::stoi(result[0].at("count")) > 0);
    
    std::cout << "testAddBooking passed!" << std::endl;
}

void testAddBookedSeats() {
    BookingRepository repository;
    auto db = DatabaseConnection::getInstance();
    
    // Lấy bookingID
    auto result = db->executeQuery("SELECT BookingID FROM BOOKING WHERE UserID = 1 LIMIT 1", {});
    assert(!result.empty());
    int bookingID = std::stoi(result[0].at("BookingID"));
    
    // Thêm ghế vào booking
    std::vector<std::string> seats = {"A1", "A2"};
    try {
        repository.addBookedSeats(bookingID, seats);
        std::cout << "Added seats to booking successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error adding seats: " << e.what() << std::endl;
        assert(false); // Test thất bại
        return;
    }
    
    // Kiểm tra xem ghế đã được thêm chưa
    result = db->executeQuery("SELECT COUNT(*) as count FROM BOOKSEAT WHERE BookingID = ?", {std::to_string(bookingID)});
    assert(!result.empty());
    assert(std::stoi(result[0].at("count")) == 2);
    
    std::cout << "testAddBookedSeats passed!" << std::endl;
}

void testViewAllBookings() {
    BookingRepository repository;
    
    // Lấy danh sách booking
    std::vector<BookingView> bookings = repository.viewAllBookings(1);
    
    // Kiểm tra kết quả
    assert(!bookings.empty());
    assert(bookings[0].movieTitle == "Movie A");
    assert(bookings[0].showTime.date == "2025-05-11");
    assert(bookings[0].showTime.startTime == "10:00");
    assert(bookings[0].showTime.endTime == "12:00");
    assert(bookings[0].bookedSeats.size() == 2);
    
    std::cout << "testViewAllBookings passed!" << std::endl;
}

int main() {
    std::cout << "Starting BookingRepository tests..." << std::endl;
    
    // Khởi tạo cơ sở dữ liệu test
    auto db = DatabaseConnection::getInstance();
    if (!db->connect("MovieTicketBookingSystem.db")) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }
    
    // Thiết lập dữ liệu test
    setupTestData(db);
    
    // Chạy các test
    testAddBooking();
    testAddBookedSeats();
    testViewAllBookings();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

