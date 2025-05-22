#include <gtest/gtest.h>
#include "../USER_pending/LoginServiceVisitor.h"
#include "../USER_pending/RegisterServiceVisitor.h"
#include "../USER_pending/BookingServiceVisitor.h"
#include "../USER_pending/Guest.h"
#include "../USER_pending/User.h"
#include "../USER_pending/Admin.h"
#include "../database/DatabaseConnection.h"
#include "../repository/AuthenticationRepositorySQL.h"
#include "../USER_pending/ServiceRegistry.h"
#include "../USER_pending/LoginService.h"
#include "../USER_pending/RegisterService.h"
#include "../USER_pending/BookingService.h"
#include "../USER_pending/IBookingService.h"

// Test: Guest có thể sử dụng dịch vụ đăng nhập và đăng ký
TEST(VisitorTest, GuestCanLoginAndRegister) {
    // Khởi tạo database connection để đăng ký services
    auto db = DatabaseConnection::getInstance();
    db->connect("database.db");
    auto repo = new AuthenticationRepositorySQL(db);
    
    // Đăng ký services vào registry trước khi test
    ServiceRegistry::addSingleton<ILoginService>(std::make_shared<LoginService>(repo));
    ServiceRegistry::addSingleton<IRegisterService>(std::make_shared<RegisterService>(repo));
    
    // Tạo guest context và test
    auto guest = std::make_shared<Guest>();

    // Test login service với visitor
    auto loginVisitor = std::make_shared<LoginServiceVisitor>();
    guest->accept(loginVisitor);
    EXPECT_NE(loginVisitor->getLoginService(), nullptr);

    // Test register service với visitor
    auto regVisitor = std::make_shared<RegisterServiceVisitor>();
    guest->accept(regVisitor);
    EXPECT_NE(regVisitor->getRegisterService(), nullptr);
    
    // Dọn dẹp
    delete repo;
    db->disconnect();
}

// Test: Guest không thể sử dụng dịch vụ đặt vé (BookingService)
TEST(VisitorTest, GuestCannotBook) {
    // Đăng ký service BookingService (để đảm bảo service tồn tại trong registry)
    ServiceRegistry::addSingleton<IBookingService>(std::make_shared<BookingService>());
    
    auto guest = std::make_shared<Guest>();
    auto bookingVisitor = std::make_shared<BookingServiceVisitor>();
    guest->accept(bookingVisitor);
    
    // Guest không nên có quyền đặt vé
    EXPECT_EQ(bookingVisitor->getBookingService(), nullptr);
}
