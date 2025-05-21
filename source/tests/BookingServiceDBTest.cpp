/*
* TEST PLAN FOR BOOKINGSERVICE
* ----------------------------
*
* 1. PURPOSE:
*    - Verify the accuracy of the BookingService class in providing ticket booking related functions.
*    - Ensure proper integration between service layer and repository layer.
*    - Confirm the completeness of booking functions, seat status viewing, and booking history viewing.
*
* 2. TEST CASES:
*    2.1. CanCreateBooking:
*         - Description: Test the function to create a new booking.
*         - Input: UserID = 2, ShowTimeID = 1, seats B1, B2, B3.
*         - Expected output: No exceptions occur during the booking process.
*         - Condition: Database has been initialized with sample data.
*
*    2.2. CanViewSeatsStatus:
*         - Description: Test the function to view seat status for a showtime.
*         - Input: ShowTimeID = 1.
*         - Expected output:
*           + Seat list is not empty.
*           + Seats A1 and A2 are booked (BOOKED).
*           + Seat A3 is available (AVAILABLE).
*           + Seats B1, B2, B3 are booked (BOOKED) after executing test case 2.1.
*         - Condition: Booking for seats B1, B2, B3 has been completed in the previous test case.
*
*    2.3. CanViewBookingHistory:
*         - Description: Test the function to view booking history of a user.
*         - Input: UserID = 2.
*         - Expected output:
*           + Booking list is not empty.
*           + Shows complete details for each booking (ID, movie title, date, time, booked seats, total price).
*         - Condition: User has at least one booking in the system.
*
* 3. TEST ENVIRONMENT SETUP:
*    - Each test run, the database will be recreated from the SQL file.
*    - BookingService is initialized with an instance of BookingRepository for each test case.
*    - The database will be closed after completing all tests.
*
* 4. TESTING METHODOLOGY:
*    - Use Google Test framework to execute the test cases.
*    - Log comprehensive information for easy analysis when errors occur.
*    - Use assertions to check actual results against expected results.
*
* 5. ASSUMPTIONS:
*    - The database.sql file contains the necessary sample data for test cases.
*    - Test cases are executed in the defined order.
*    - No external interference with the database during testing.
*/

#include <gtest/gtest.h>
#include "../service/IBookingService.h"
#include "../service/BookingService.h"
#include "../repository/SeatView.h"
#include <iostream>
#include <string>
#include <filesystem>

// Test Case 2.1: Test creating a new booking
TEST(BookingServiceTest, CanCreateBooking) {
    // Step 1: Initialize service with repository
    std::shared_ptr<IBookingService> service = std::make_shared<BookingService>(std::make_shared<BookingRepository>("database.db"));
    
    // Step 2: Prepare input data
    int userID = 2;
    int showTimeID = 1;
    std::vector<std::string> seats = {"B1", "B2", "B3"};
    
    // Step 3: Execute booking and verify no exceptions occur    
    EXPECT_NO_THROW(service->createBooking(userID, showTimeID, seats)) 
        << "The booking creation process should not throw exceptions";
}

// Test Case 2.2: Test viewing seat status
TEST(BookingServiceTest, CanViewSeatsStatus) {
    // Step 1: Initialize service with repository
    std::shared_ptr<IBookingService> service = std::make_shared<BookingService>(std::make_shared<BookingRepository>("database.db"));
    
    // Step 2: Call viewSeatsStatus for showtime 1
    int showTimeID = 1;
    std::vector<SeatView> seatsStatus = service->viewSeatsStatus(showTimeID);
    
    // Print seat information for debugging
    std::cout << "Seats status for showTimeID " << showTimeID << ": ";
    for (const auto& seat : seatsStatus) {
        std::cout << "Seat ID: " << seat.seat->id() << ", Status: " << (seat.status == BOOKED ? "BOOKED" : "AVAILABLE") << std::endl;
    }
      // Step 3: Verify the seat list is not empty
    EXPECT_FALSE(seatsStatus.empty()) << "Seat list should not be empty";    // Step 4: Check status of seat A1 (should be BOOKED)
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "A1") {
            return seat.status == BOOKED;
        }
        return false;    }) != seatsStatus.end()) << "Seat A1 should have status BOOKED";
    
    // Step 5: Check status of seat A2 (should be BOOKED)
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "A2") {
            return seat.status == BOOKED;
        }
        return false;    }) != seatsStatus.end()) << "Seat A2 should have status BOOKED";
    
    // Step 6: Check status of seat A3 (should be AVAILABLE)
    EXPECT_TRUE(std::find_if(seatsStatus.begin(), seatsStatus.end(), [](const SeatView& seat) -> bool {
        if (seat.seat->id() == "A3") {
            return seat.status == AVAILABLE;
        }
        return false;
    }) != seatsStatus.end()) << "Seat A3 should have status AVAILABLE";
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
}

// Test Case 2.3: Test viewing booking history
TEST(BookingServiceTest, CanViewBookingHistory) {
    // Step 1: Initialize service with repository
    std::shared_ptr<IBookingService> service = std::make_shared<BookingService>(std::make_shared<BookingRepository>("database.db"));
    
    // Step 2: Call viewBookingHistory for user 2
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
    // Step 3: Verify booking list is not empty
    EXPECT_FALSE(bookings.empty()) << "Booking list should not be empty";
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