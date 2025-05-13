#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../repository/IBookingRepository.h"
#include "../repository/BookingView.h"
#include "../repository/SeatView.h"

// Mock class cho IBookingRepository
class MockBookingRepository : public IBookingRepository {
public:
    MOCK_METHOD(void, addBooking, (const int&, const int&), (override));
    MOCK_METHOD(void, addBookedSeats, (const int&, const std::vector<std::string>&), (override));
    MOCK_METHOD(std::vector<BookingView>, viewAllBookings, (const int&), (override));
    MOCK_METHOD(std::vector<SeatView>, viewSeatsStatus, (const int&), (override));
};

TEST(MockBookingRepositoryTest, AddBookingCalled) {
    MockBookingRepository mockRepo;
    EXPECT_CALL(mockRepo, addBooking(1, 1)).Times(1);
    mockRepo.addBooking(1, 1);
}

TEST(MockBookingRepositoryTest, ViewAllBookingsReturnFakeData) {
    MockBookingRepository mockRepo;
    std::vector<BookingView> fakeResult; // dữ liệu giả
    EXPECT_CALL(mockRepo, viewAllBookings(1)).WillOnce(::testing::Return(fakeResult));
    auto result = mockRepo.viewAllBookings(1);
    ASSERT_EQ(result.size(), 0);
}

TEST(MockBookingRepositoryTest, ViewSeatsStatusReturnFakeData) {
    MockBookingRepository mockRepo;
    std::vector<SeatView> fakeSeats; // dữ liệu giả
    EXPECT_CALL(mockRepo, viewSeatsStatus(1)).WillOnce(::testing::Return(fakeSeats));
    auto result = mockRepo.viewSeatsStatus(1);
    ASSERT_EQ(result.size(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
