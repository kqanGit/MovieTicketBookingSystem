#include <gtest/gtest.h>
#include "../SessionManager.h"
#include "../USER_pending/Guest.h"
#include "../USER_pending/User.h"
#include "../USER_pending/Admin.h"
#include "../USER_pending/UserInformationService.h"

// Test Guest context: không có UserInformationService
TEST(SessionManagerTest, GuestHasNoUserInfoService) {
    SessionManager manager;

    auto context = manager.getCurrentContext();
    auto info = context->getUserInformationService();

    EXPECT_EQ(info, nullptr);  // Guest không có thông tin người dùng
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
}

// Test User context: có info và role là "user"
TEST(SessionManagerTest, UserHasCorrectRoleInfo) {
    AccountInformation acc = { "user01", "pass", "012", "user01@gmail.com", "User" };

    SessionManager manager;
    ASSERT_TRUE(manager.setUserContext(acc));

    auto info = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(info, nullptr);
    EXPECT_EQ(info->getRole(), "User");
    EXPECT_EQ(manager.getCurrentRole(), "User");
}

// Test Admin context: có info và role là "admin"
TEST(SessionManagerTest, AdminHasCorrectRoleInfo) {
    AccountInformation acc = { "admin01", "pass", "099", "admin@gmail.com", "Admin" };

    SessionManager manager;
    ASSERT_TRUE(manager.setUserContext(acc));

    auto info = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(info, nullptr);
    EXPECT_EQ(info->getRole(), "Admin");
    EXPECT_EQ(manager.getCurrentRole(), "Admin");
}