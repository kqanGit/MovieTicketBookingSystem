#include <gtest/gtest.h>
#include "../USER_pending/UserInformationService.h"

TEST(UserInfoTest, PrintAccountInfoOutputsCorrectData) {
    AccountInformation acc = { "Thao", "123", "0909", "thao@gmail.com", "user" };
    UserInformationService info(acc);

    testing::internal::CaptureStdout();
    info.printAccountInfo();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Thao"), std::string::npos);
    EXPECT_NE(output.find("thao@gmail.com"), std::string::npos);
}
