#include "BookingRepositorySQL.h"
#include "SeatFactory.h"

BookingRepository::BookingRepository() {
    _dbConnection = DatabaseConnection::getInstance();
    if (!_dbConnection->connect("MovieTicketBookingSystem.db")) {
        throw std::runtime_error("Failed to connect to the database.");
    }
}

BookingRepository::~BookingRepository() {
    // Không cần giải phóng _dbConnection vì nó là singleton
    // DatabaseConnection sẽ tự quản lý vòng đời của nó
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
    auto result = _dbConnection->executeQuery(sql_stmt, params);    std::map<int, Booking> bookingDetails;
    std::map<int, std::vector<std::shared_ptr<ISeat>>> seatsByBooking;
    for (const auto& row : result) {
        int bookingID = std::stoi(row.at("BookingID"));
        if (bookingDetails.find(bookingID) == bookingDetails.end()) {
            // Sử dụng emplace thay vì operator[] để tránh cần constructor mặc định
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
                        row.at("StartTime"),  // Sửa StarTime thành StartTime
                        row.at("EndTime")
                    )
                )
            );
        }        // Chuyển đổi SeatType từ chuỗi thành enum
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
        ISeat* temp = seatFactory.createSeat(row.at("SeatID"), seatType, price);
        std::shared_ptr<ISeat> seat(temp);
        seatsByBooking[bookingID].push_back(seat);
    }    std::vector<BookingView> bookings;
    for (const auto& pair : bookingDetails) {
        int bookingID = pair.first;
        Booking booking = pair.second;
        float totalPrice = 0;
        for (const auto& seat : seatsByBooking[bookingID]) {
            totalPrice += seat->price();
        }        
          bookings.emplace_back(
            bookingID,  // Không cần chuyển đổi int sang string
            booking.movieID,
            booking.movieTitle,
            booking.showTime,
            seatsByBooking[bookingID],
            totalPrice
        );
    }

    return bookings;
}