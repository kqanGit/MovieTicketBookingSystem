#include <gtest/gtest.h>
#include "../repository/IBookingRepository.h"
#include "../repository/BookingRepositorySQL.h"
#include "../repository/BookingView.h"
#include "../repository/SeatView.h"
#include "../database/DatabaseConnection.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <filesystem>

class BookingRepositoryDBTest : public ::testing::Test {
protected:
    IBookingRepository* repo;
    
    void SetUp() override {
        repo = new BookingRepository(); 
    }
    
    void TearDown() override {
        delete repo;
        // Có thể xóa file database test sau khi test xong
        // std::remove(dbPath.c_str());
    }
};

// Test xem có lấy được danh sách booking hay không
TEST_F(BookingRepositoryDBTest, CanViewBookings) {
    // Lấy danh sách booking của user 1 (từ dữ liệu mẫu)
    auto bookings = repo->viewAllBookings(1);
    
    // Kiểm tra có dữ liệu trả về
    ASSERT_FALSE(bookings.empty());
      // Kiểm tra booking đầu tiên có đúng dữ liệu không
    EXPECT_EQ(bookings.size(), 1);
    EXPECT_EQ(bookings[0].bookingID, 1);
    EXPECT_EQ(bookings[0].showTime.date, "2025-05-10");
    EXPECT_EQ(bookings[0].showTime.startTime, "18:00");
    EXPECT_EQ(bookings[0].showTime.endTime, "20:30");
    EXPECT_EQ(bookings[0].movieID, 1);
    EXPECT_EQ(bookings[0].movieTitle, "Avengers");
    
    // Sử dụng một biến trung gian để gọi phương thức id()
    std::string seatId = bookings[0].bookedSeats[0]->id();
    EXPECT_EQ(seatId, "A1");
    seatId = bookings[0].bookedSeats[1]->id();
    EXPECT_EQ(seatId, "A2");
}

// Test xem trạng thái ghế
TEST_F(BookingRepositoryDBTest, CanViewSeatStatus) {
    // Lấy trạng thái ghế cho showtime 1
    std::cout << "Calling viewSeatsStatus for showTimeID = 1" << std::endl;
    auto seats = repo->viewSeatsStatus(1);
    
    // Kiểm tra có đủ 6 ghế không
    std::cout << "NUMBER OF SEAT: " << seats.size() << std::endl;
    EXPECT_GE(seats.size(), 6);
    
    // Kiểm tra ghế A1 và A2 đã được đặt (từ dữ liệu mẫu)
    bool foundA1 = false;
    bool foundA2 = false;
    
    for (const auto& seat : seats) {
        std::cout << "Checking seat... ";
        if (seat.seat == nullptr) {
            std::cout << "WARNING: seat pointer is null!" << std::endl;
            continue;
        }
        std::string seatId = seat.seat->id();
        std::cout << "Seat ID: " << seatId << ", Status: " << (seat.status == BOOKED ? "BOOKED" : "AVAILABLE") << std::endl;
        if (seatId == "A1") {
            foundA1 = true;
            EXPECT_EQ(seat.status, BOOKED);
        }
        else if (seatId == "A2") {
            foundA2 = true;
            EXPECT_EQ(seat.status, BOOKED);
        }
        else if (seatId == "A3") {
            // A3 chưa được đặt trong data mẫu
            EXPECT_EQ(seat.status, AVAILABLE);
        }
    }
    
    EXPECT_TRUE(foundA1);
    EXPECT_TRUE(foundA2);
}

// Test thêm ghế đã đặt cho booking
TEST_F(BookingRepositoryDBTest, CanAddBookedSeats) {
    // Thêm booking mới
    repo->addBooking(1, 2);
    
    // Giả sử bookingID mới là 3 (1 và 2 đã có trong dữ liệu mẫu)
    int newBookingId = 3;
    
    // Đặt ghế A3 và B3 cho booking này
    std::vector<std::string> seats = {"A3", "B3"};
    repo->addBookedSeats(newBookingId, seats);
    
    // Kiểm tra trạng thái ghế cho showtime 2
    auto seatsStatus = repo->viewSeatsStatus(2);
    
    // Tìm ghế A3 và B3, chúng phải có trạng thái BOOKED
    bool foundA3 = false;
    bool foundB3 = false;
    
    for (const auto& seat : seatsStatus) {
        std::string seatId = seat.seat->id();
        if (seatId == "A3") {
            foundA3 = true;
            EXPECT_EQ(seat.status, BOOKED);
        }
        else if (seatId == "B3") {
            foundB3 = true;
            EXPECT_EQ(seat.status, BOOKED);
        }
    }
    
    EXPECT_TRUE(foundA3);
    EXPECT_TRUE(foundB3);
}

int main(int argc, char** argv) {

    auto db = DatabaseConnection::getInstance();

     const std::string dbPath = "database.db";
    
    // Remove the existing database file if it exists
    if (std::filesystem::exists(dbPath)) {
        std::cout << "Removing existing database file..." << std::endl;
        std::filesystem::remove(dbPath);
    }


    if (!db->connect(dbPath)) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }   
    db->executeSQLFile("database.sql");
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    
    return result;
}
