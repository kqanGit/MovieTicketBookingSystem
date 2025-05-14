#include <gtest/gtest.h>
#include "../service/IBookingService.h"
#include "../service/BookingService.h"
#include "../repository/SeatView.h"
#include <iostream>
#include <string>
#include <filesystem>

TEST(BookingServiceTest, CanCreateBooking) {
    IBookingService* service = new BookingService();
    int userID = 2;
    int showTimeID = 1;
    std::vector<std::string> seats = {"B1", "B2", "B3"};
    EXPECT_NO_THROW(service->createBooking(userID, showTimeID, seats));
    delete service;
}

TEST(BookingServiceTest, CanViewSeatsStatus) {
    IBookingService* service = new BookingService();
    int showTimeID = 1;
    std::vector<SeatView> seatsStatus = service->viewSeatsStatus(showTimeID);
    std::cout << "Seats status for showTimeID " << showTimeID << ": ";
    for (const auto& seat : seatsStatus) {
        std::cout << "Seat ID: " << seat.seat->id() << ", Status: " << (seat.status == BOOKED ? "BOOKED" : "AVAILABLE") << std::endl;
    }
    EXPECT_FALSE(seatsStatus.empty());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "A1") {
            return seat.status == BOOKED;
        }
        return false;
    }) != seatsStatus.end());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "A2") {
            return seat.status == BOOKED;
        }
        return false;
    }) != seatsStatus.end());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "A3") {
            return seat.status == AVAILABLE;
        }
        return false;
    }) != seatsStatus.end());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool{
        if (seat.seat->id() == "A1") {
            return seat.status == BOOKED;
        }
        return false;   
    }) != seatsStatus.end());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "B1") {
            return seat.status == BOOKED;
        }
        return false;
    }) != seatsStatus.end());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "B2") {
            return seat.status == BOOKED;
        }
        return false;
    }) != seatsStatus.end());
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "B3") {
            return seat.status == BOOKED;
        }
        return false;
    }) != seatsStatus.end());
    delete service;
}

TEST(BookingServiceTest, CanViewBookingHistory) {
    IBookingService* service = new BookingService();
    int userID = 2;
    std::vector<BookingView> bookings = service->viewBookingHistory(userID);
    std::cout << "Booking history for userID " << userID << ": " << std::endl;
    for (const auto& booking : bookings) {
        std::cout << "Booking ID: " << booking.bookingID << ", Movie Title: " << booking.movieTitle
                  << ", Date: " << booking.showTime.date << ", Start Time: " << booking.showTime.startTime
                  << ", End Time: " << booking.showTime.endTime << ", Seats: ";
        for (const auto& seat : booking.bookedSeats) {
            std::cout << seat->id() << " ";
        }
        std::cout << ", Total Price: " << booking.totalPrice << std::endl;
    }
    EXPECT_FALSE(bookings.empty());
    delete service;
}

int main(int argc, char **argv) {
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

    db->disconnect();
    return result;
}