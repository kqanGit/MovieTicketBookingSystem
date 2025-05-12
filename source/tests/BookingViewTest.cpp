#include "../repository/BookingView.h"
#include "../model/ISeat.h"
#include "../model/ShowTime.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "Testing BookingView...\n";
    
    // Tạo một ShowTime object
    ShowTime showTime(1, "2025-05-11", "10:00", "12:00");
      // Tạo một BookingView object
    BookingView bookingView(
        1,
        1,
        "Movie A",
        showTime,
        std::vector<std::shared_ptr<ISeat>>(),
        0.0f
    );


    // Test the BookingView constructor
    assert(bookingView.bookingID == 1);
    assert(bookingView.movieID == 1);
    assert(bookingView.movieTitle == "Movie A");
    assert(bookingView.showTime.showTimeID == 1);
    assert(bookingView.showTime.date == "2025-05-11");
    assert(bookingView.showTime.startTime == "10:00");
    assert(bookingView.showTime.endTime == "12:00");
    assert(bookingView.totalPrice == 0.0f);
    assert(bookingView.bookedSeats.empty());

    // print bookingView details
    std::cout << "Booking ID: " << bookingView.bookingID << std::endl;
    std::cout << "Movie ID: " << bookingView.movieID << std::endl;
    std::cout << "Movie Title: " << bookingView.movieTitle << std::endl;
    std::cout << "Show Time ID: " << bookingView.showTime.showTimeID << std::endl;
    std::cout << "Show Time Date: " << bookingView.showTime.date << std::endl;
    std::cout << "Show Time Start: " << bookingView.showTime.startTime << std::endl;
    std::cout << "Show Time End: " << bookingView.showTime.endTime << std::endl;
    std::cout << "Total Price: " << bookingView.totalPrice << std::endl;
    std::cout << "Booked Seats: " << bookingView.bookedSeats.size() << std::endl;
    for (const auto& seat : bookingView.bookedSeats) {
        std::cout << "Seat ID: " << seat->id() << ", Price: " << seat->price() << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;

}