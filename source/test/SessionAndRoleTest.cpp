#include <gtest/gtest.h>
#include "../USER_pending/User.h"
#include "../USER_pending/Admin.h"
#include "../USER_pending/Guest.h"
#include "../SessionManager.h"
#include "../USER_pending/UserInformationService.h"

TEST(SessionTest, CreateUserAndAdminContexts) {
    AccountInformation userAcc = { "u", "p", "p", "u@gmail.com", "User" };
    AccountInformation adminAcc = { "a", "p", "p", "a@gmail.com", "Admin" };

    auto user = std::make_unique<User>(userAcc);
    auto admin = std::make_unique<Admin>(adminAcc);

    // Kiểm tra vai trò của user và admin thông qua infoService
    EXPECT_EQ(user->getUserInformationService()->getAccountType(), "User");
    EXPECT_EQ(admin->getUserInformationService()->getAccountType(), "Admin");
}

TEST(SessionManagerTest, LoginAndLogout) {
    SessionManager manager;
    AccountInformation info = { "x", "x", "0", "x@gmail.com", "User" };
    EXPECT_TRUE(manager.setUserContext(info));
    EXPECT_EQ(manager.getCurrentRole(), "User");
    EXPECT_TRUE(manager.logout());
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
}
