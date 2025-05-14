#include "IBookingRepository.h"
#include "DatabaseConnection.h"
#include "ShowTime.h"
#include "Booking.h"
#include <stdexcept>
#include <map>
#include <format>
class BookingRepository : public IBookingRepository {
private:
    DatabaseConnection* _dbConnection;
public:
    BookingRepository (std::string dbFilePath);
    void addBooking(const int& userID, const int& showTimeID) override;
    void addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) override;
    int getLatestBookingID(const int& userID) override;
    std::vector<BookingView> viewAllBookings(const int& userID) override;
    std::vector<SeatView> viewSeatsStatus(const int& showTimeID) override;
    
    ~BookingRepository();
};