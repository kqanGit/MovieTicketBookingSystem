#include "IBookingRepository.h"
#include "DatabaseConnection.h"
#include "BookingView.h"
#include "Booking.h"
#include "ShowTime.h"
#include <stdexcept>
#include <map>
#include <format>
class BookingRepository : public IBookingRepository {
private:
    DatabaseConnection* _dbConnection;
public:
    BookingRepository ();
    void addBooking(const int& userID, const int& showTimeID) override;
    void addBookedSeats(const int& bookingID, const std::vector<std::string>& bookedSeats) override;
    std::vector<BookingView> viewAllBookings(const int& userID) override;
    ~BookingRepository();
};