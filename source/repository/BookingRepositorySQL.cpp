#include "BookingRepositorySQL.h"
BookingRepository::BookingRepository() {
    dbConnection = DatabaseConnection::getInstance();
    if (!dbConnection->connect("movie.db")) {
        throw std::runtime_error("Failed to connect to the database.");
    }
}

void BookingRepository::addBooking(const int& userID, const int& showTimeID) {
    std::string sql_stmt = "insert into booking (UserID, ShowTimeID) values (?, ?)";
    std::vector<std::string> params = {std::to_string(userID), std::to_string(showTimeID)};
    if (!dbConnection->executeNonQuery(sql_stmt, params)) {
        throw std::runtime_error(
            std::format("fail to create booking for showTimeID : {}, please try again later.\n", showTimeID)
        );
    }
}


void BookingRepository::addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) {
    std::string sql_stmt = "insert into BOOKEDSEAT (BookingID, SeatID) values (?, ?)";
    for (const auto& seatID : bookedSeats) {
        std::vector<std::string> params = {std::to_string(bookingID), seatID};  
        if (!dbConnection->executeNonQuery(sql_stmt, params)) {
            throw std::runtime_error(
                std::format("Failed to book seat {}\n", seatID)
            );
        }
    }
}

std::vector<Booking> BookingRepository::viewAllBookings(const int& userID) const {
    std::string sql_stmt = "select * from booking where UserID = ?";
    std::vector<std::string> params = {std::to_string(userID)};
    auto results = dbConnection->executeQuery(sql_stmt, params);
    std::vector<Booking> bookings;
    
    if (results.empty()) {
        return bookings; // No bookings found
    }

    for (const auto& row : results) {
        int bookingID = std::stoi(row.at("BookingID"));
        int showTimeID = std::stoi(row.at("ShowTimeID"));
        bookings.emplace_back(bookingID, userID, showTimeID);
    }
}
