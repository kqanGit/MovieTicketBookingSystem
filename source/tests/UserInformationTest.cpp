#include <gtest/gtest.h>
#include "../service/UserInformationService.h"

// Test kiểm tra hàm printAccountInfo xuất ra đúng thông tin tài khoản
TEST(UserInfoTest, PrintAccountInfoOutputsCorrectData) {
    AccountInformation acc = { "Thao", "123", "0909", "thao@gmail.com", "user" };
    UserInformationService info(acc);

    // Ghi lại toàn bộ output ra stdout khi gọi printAccountInfo
    testing::internal::CaptureStdout();
    info.printAccountInfo();
    std::string output = testing::internal::GetCapturedStdout();

    // Kiểm tra output có chứa tên và email đúng không
    EXPECT_NE(output.find("Thao"), std::string::npos);              // Có tên "Thao" trong output
    EXPECT_NE(output.find("thao@gmail.com"), std::string::npos);    // Có email "thao@gmail.com" trong output
}
