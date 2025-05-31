#include "BookingRepositorySQL.h"
#include "../model/SeatFactory.h"
#include "../model/Booking.h"

BookingRepository::BookingRepository(std::string dbFilePath) {
    _dbConnection = DatabaseConnection::getInstance();
    if (!_dbConnection->connect(dbFilePath)) {
        throw std::runtime_error("Failed to connect to the database.");
    }
}

BookingRepository::~BookingRepository() {

}

void BookingRepository::addBooking(const int& userID, const int& showTimeID) {
    std::string sql_stmt = "insert into booking (UserID, ShowTimeID) values (?, ?)";
    std::vector<std::string> params = {std::to_string(userID), std::to_string(showTimeID)};
    if (!_dbConnection->executeNonQuery(sql_stmt, params)) {
        throw std::runtime_error(
            std::format("fail to create booking for showTimeID : {}, please try again later.\n", showTimeID)
        );
    }
}


void BookingRepository::addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) {
    std::string sql_stmt = "insert into BOOKSEAT (BookingID, SeatID) values (?, ?)";
    for (const auto& seatID : bookedSeats) {
        std::vector<std::string> params = {std::to_string(bookingID), seatID};  
        if (!_dbConnection->executeNonQuery(sql_stmt, params)) {
            throw std::runtime_error(
                std::format("Failed to book seat {}\n", seatID)
            );
        }
    }
}

std::vector<BookingView> BookingRepository::viewAllBookings(const int& userID) {
    std::string sql_stmt = "select b.BookingID, st.ShowTimeID, st.Date, st.StartTime, st.EndTime, m.Title, m.MovieID, bs.SeatID, s.SeatType, s.Price "
                           "from BOOKING b "
                           "join SHOWTIME st on st.ShowTimeID = b.ShowTimeID "
                           "join MOVIE m on m.MovieID = st.MovieID "
                           "join BOOKSEAT bs on bs.BookingID = b.BookingID "
                           "join SEAT s on s.SeatID = bs.SeatID "
                           "where b.UserID = ?";
    std::vector<std::string> params = {std::to_string(userID)};
    auto result = _dbConnection->executeQuery(sql_stmt, params);    
    std::map<int, Booking> bookingDetails;
    std::map<int, std::vector<std::shared_ptr<ISeat>>> seatsByBooking;
    
    for (const auto& row : result) {
        int bookingID = std::stoi(row.at("BookingID"));
        if (bookingDetails.find(bookingID) == bookingDetails.end()) {
          
            bookingDetails.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(bookingID),
                std::forward_as_tuple(
                    bookingID,
                    std::stoi(row.at("MovieID")),
                    row.at("Title"),
                    ShowTime(
                        std::stoi(row.at("ShowTimeID")),
                        row.at("Date"),
                        row.at("StartTime"), 
                        row.at("EndTime")
                    )
                )
            );
        }      

        SeatType seatType;
        std::string seatTypeStr = row.at("SeatType");
        if (seatTypeStr == "Single") {
            seatType = SeatType::SINGLE;
        } else if (seatTypeStr == "Couple") {
            seatType = SeatType::COUPLE;
        } else {
            seatType = SeatType::SINGLE;
        }
        
        float price = std::stof(row.at("Price"));
        SeatFactory seatFactory;
        ISeat* temp = seatFactory.createSeat(row.at("SeatID"), seatType, price);
        std::shared_ptr<ISeat> seat(temp);
        seatsByBooking[bookingID].push_back(seat);
    }  

    std::vector<BookingView> bookings;
    for (const auto& pair : bookingDetails) {
        int bookingID = pair.first;
        Booking booking = pair.second;
        float totalPrice = 0;
        for (const auto& seat : seatsByBooking[bookingID]) {
            totalPrice += seat->price();
        }        
          bookings.emplace_back(
            bookingID, 
            booking.movieID,
            booking.movieTitle,
            booking.showTime,
            seatsByBooking[bookingID],
            totalPrice
        );
    }

    return bookings;
}

std::vector<SeatView> BookingRepository::viewSeatsStatus(const int& showTimeID) {
    std::string sql_stmt = "select SeatID, SeatType, Price from SEAT";
    auto seatInfo = _dbConnection->executeQuery(sql_stmt, {});
    sql_stmt = "select SeatID from BOOKSEAT "
            "join BOOKING b on b.BookingID = BOOKSEAT.BookingID "
            "where ShowTimeID = ?";
    std::vector<std::string> params = {std::to_string(showTimeID)};
    auto bookedSeats = _dbConnection->executeQuery(sql_stmt, params);
    std::map<std::string, bool> bookedSeatsMap;
    for (const auto& row : bookedSeats) {
        bookedSeatsMap[row.at("SeatID")] = true;
    }    
    std::map<std::string, SeatView> seatDetails;
    for (const auto& row : seatInfo) {
        std::string seatID = row.at("SeatID");
        SeatType seatType;
        std::string seatTypeStr = row.at("SeatType");
        if (seatTypeStr == "Single") {
            seatType = SeatType::SINGLE;
        } else if (seatTypeStr == "Couple") {
            seatType = SeatType::COUPLE;
        } else {
            seatType = SeatType::SINGLE; // Giá trị mặc định
        }
        
        float price = std::stof(row.at("Price"));
        SeatFactory seatFactory;
        ISeat* temp = seatFactory.createSeat(seatID, seatType, price);
        std::shared_ptr<ISeat> seat(temp);
        SeatStatus status = bookedSeatsMap[seatID] ? SeatStatus::BOOKED : SeatStatus::AVAILABLE;
        seatDetails.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(seatID),
            std::forward_as_tuple(seat, status)
        );
    }
    std::vector<SeatView> seatsView;
    for (const auto& pair : seatDetails) {
        seatsView.push_back(pair.second);
    }
    return seatsView;
}

int BookingRepository::getLatestBookingID(const int& userID) {
    std::string sql_stmt = "select BookingID from BOOKING where UserID = ? order by BookingID desc limit 1";
    std::vector<std::string> params = {std::to_string(userID)};
    auto result = _dbConnection->executeQuery(sql_stmt, params);
    if (result.empty()) {
        return 0; // No bookings found
    }
    return std::stoi(result[0].at("BookingID"));
}