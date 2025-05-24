/*
* TEST PLAN FOR BOOKINGREPOSITORY
* ------------------------------
*
* 1. PURPOSE:
*    - Verify the accuracy of the BookingRepository class in interacting with the database.
*    - Ensure that booking, seat status viewing, and booking history viewing functions work correctly.
*
* 2. TEST CASES:
*    2.1. CanViewBookings:
*         - Description: Test the function to view the booking list of a specific user.
*         - Input: UserID = 1.
*         - Expected output: Booking list is not empty, contains correct information about showtime, movie, and booked seats.
*         - Condition: Sample data already has bookings for user 1.
*
*    2.2. CanViewSeatStatus:
*         - Description: Test the function to view seat status for a showtime.
*         - Input: ShowTimeID = 1.
*         - Expected output: Seat list with correct status (A1, A2 are booked, A3 is available).
*         - Condition: Sample data already has seat information for showtime 1.
*
*    2.3. CanAddBookedSeats:
*         - Description: Test the function to book seats for a booking.
*         - Input: BookingID = 3, seats A3 and B3.
*         - Expected output: Seats A3 and B3 have status BOOKED after execution.
*         - Condition: A new booking with ID = 3 has been created.
*
*    2.4. CanGetLatestBookingID:
*         - Description: Test the function to get the latest booking ID of a user.
*         - Input: UserID = 1.
*         - Expected output: The latest booking ID is 3.
*         - Condition: Previous booking operations have been performed.
*
* 3. TEST ENVIRONMENT SETUP:
*    - Each test run, the database will be recreated from the SQL file.
*    - Use fixture to initialize the repository before each test.
*    - The database will be closed after all tests are completed.
*
* 4. ASSUMPTIONS:
*    - The database.sql file contains the necessary sample data for the test cases.
*    - There is no external interference with the database during testing.
*/

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
#include <memory>

class BookingRepositoryDBTest : public ::testing::Test {
protected:
    std::shared_ptr<IBookingRepository> repo;
    
    void SetUp() override {
        repo = std::make_shared<BookingRepository>("database.db");
    }
    
    void TearDown() override {
    }
};

// Test Case 2.1: Test viewing booking list
TEST_F(BookingRepositoryDBTest, CanViewBookings) {
    // Step 1: Call viewAllBookings to get the booking list for user 1
    auto bookings = repo->viewAllBookings(1);
    
    // Step 2: Check that the list is not empty (should have at least one booking)
    ASSERT_FALSE(bookings.empty());      // Step 3: Check the number of bookings and details of the first booking
    EXPECT_EQ(bookings.size(), 1);
    EXPECT_EQ(bookings[0].bookingID, 1) << "BookingID should be 1";
    EXPECT_EQ(bookings[0].showTime.date, "2025-05-10") << "Show date should be 2025-05-10";
    EXPECT_EQ(bookings[0].showTime.startTime, "18:00") << "Start time should be 18:00";
    EXPECT_EQ(bookings[0].showTime.endTime, "20:30") << "End time should be 20:30";
    EXPECT_EQ(bookings[0].movieID, 1) << "MovieID should be 1";
    EXPECT_EQ(bookings[0].movieTitle, "Avengers") << "Movie title should be Avengers";
      // Step 4: Check booked seat information
    std::string seatId = bookings[0].bookedSeats[0]->id();
    EXPECT_EQ(seatId, "A1") << "First seat should be A1";
    seatId = bookings[0].bookedSeats[1]->id();
    EXPECT_EQ(seatId, "A2") << "Second seat should be A2";
}

// Test Case 2.2: Test viewing seat status
TEST_F(BookingRepositoryDBTest, CanViewSeatStatus) {
    // Step 1: Call viewSeatsStatus to get seat status for showtime 1
    std::cout << "Calling viewSeatsStatus for showTimeID = 1" << std::endl;
    auto seats = repo->viewSeatsStatus(1);
    
    // Step 2: Check the number of seats (should have at least 6 seats)
    std::cout << "NUMBER OF SEAT: " << seats.size() << std::endl;
    EXPECT_GE(seats.size(), 6) << "There should be at least 6 seats for the showtime";      // Step 3: Check the status of seats A1 and A2 (already booked in sample data)
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
        
        // Step 4: Check each specific seat
        if (seatId == "A1") {
            foundA1 = true;
            EXPECT_EQ(seat.status, BOOKED) << "Seat A1 should have status BOOKED";
        }
        else if (seatId == "A2") {
            foundA2 = true;
            EXPECT_EQ(seat.status, BOOKED) << "Seat A2 should have status BOOKED";
        }
        else if (seatId == "A3") {
            // A3 is not booked in the sample data
            EXPECT_EQ(seat.status, AVAILABLE) << "Seat A3 should have status AVAILABLE";
        }
    }
      // Step 5: Check that the required seats were found
    EXPECT_TRUE(foundA1) << "Seat A1 not found in the list";
    EXPECT_TRUE(foundA2) << "Seat A2 not found in the list";
}

// Test Case 2.3: Test booking seats for a booking
TEST_F(BookingRepositoryDBTest, CanAddBookedSeats) {
    // Step 1: Create a new booking for user 1, showtime 2
    repo->addBooking(1, 2);
    
    // Step 2: Determine the new bookingID (based on sample data, the new ID will be 3)
    int newBookingId = 3;
    
    // Step 3: Book seats A3 and B3 for this booking
    std::vector<std::string> seats = {"A3", "B3"};
    repo->addBookedSeats(newBookingId, seats);      // Step 4: Check seat status for showtime 2 after booking
    auto seatsStatus = repo->viewSeatsStatus(2);
    
    // Step 5: Find and check the status of seats A3 and B3 (should be BOOKED)
    bool foundA3 = false;
    bool foundB3 = false;
    
    for (const auto& seat : seatsStatus) {
        std::string seatId = seat.seat->id();
        if (seatId == "A3") {
            foundA3 = true;
            EXPECT_EQ(seat.status, BOOKED) << "Seat A3 should have status BOOKED after booking";
        }
        else if (seatId == "B3") {
            foundB3 = true;
            EXPECT_EQ(seat.status, BOOKED) << "Seat B3 should have status BOOKED after booking";
        }
    }
    
    // Step 6: Confirm that both seats were found
    EXPECT_TRUE(foundA3) << "Seat A3 not found in the list";
    EXPECT_TRUE(foundB3) << "Seat B3 not found in the list";
}

// Test Case 2.4: Test getting the latest booking ID
TEST_F(BookingRepositoryDBTest, CanGetLatestBookingID) {
    // Step 1: Call getLatestBookingID for user 1
    int latestBookingId = repo->getLatestBookingID(1);
    
    // Step 2: Check that the latest ID is correct (based on previous test cases)
    EXPECT_EQ(latestBookingId, 3) << "The latest booking ID should be 3";
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
    
    db->disconnect();
    return result;
}

