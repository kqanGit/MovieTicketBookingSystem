#include "IBookingRepositorySQL.h"
#include "DatabaseConnection.h"
#include <stdexcept>
#include <format>
class BookingRepository : public IBookingRepository {
private:
    DatabaseConnection* dbConnection;
public:
    BookingRepository ();
    void addBooking(const int& userID, const int& showTimeID) override;
    void addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) override;
    std::vector<Booking> viewAllBookings(const int& userID) const override;
};