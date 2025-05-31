/*
* TEST PLAN FOR USER INFORMATION SERVICE
* =====================================
*
* 1. PURPOSE:
*    - Verify the functionality of UserInformationService class
*    - Ensure proper display and management of user account information
*    - Test output formatting and data integrity
*
* 2. TEST SCOPE:
*    - Account information display functionality
*    - Output format validation
*    - Data consistency checks
*    - Service behavior with different user roles
*
* 3. TEST CASES:
*    3.1. PrintAccountInfoOutputsCorrectData:
*         - Description: Verify that printAccountInfo() outputs the correct user information
*         - Input: AccountInformation with username "Thao", email "thao@gmail.com"
*         - Expected Output: Console output contains both username and email
*         - Test Method: Capture stdout and verify content presence
*         - Validation: Check that specific strings exist in output
*
* 4. FUTURE TEST CASES (TO BE IMPLEMENTED):
*    4.1. PrintAccountInfoWithEmptyData:
*         - Test behavior with empty or null account information
*    4.2. PrintAccountInfoForDifferentRoles:
*         - Test output formatting for User, Admin, Guest roles
*    4.3. PrintAccountInfoWithSpecialCharacters:
*         - Test handling of special characters in user data
*    4.4. GetAccountInformationReturnsCorrectData:
*         - Test retrieval of account information without printing
*    4.5. UpdateAccountInformationTest:
*         - Test updating user information (if supported)
*
* 5. TEST DATA:
*    - Sample user: "Thao", password "123", phone "0909", email "thao@gmail.com", role "user"
*    - Uses AccountInformation struct for data input
*
* 6. TESTING METHODOLOGY:
*    - Use Google Test framework assertions
*    - Capture stdout using testing::internal::CaptureStdout()
*    - Verify string presence using std::string::find()
*    - Use EXPECT_NE with std::string::npos for content validation
*
* 7. DEPENDENCIES:
*    - UserInformationService class
*    - AccountInformation struct
*    - Google Test framework
*
* 8. ASSUMPTIONS:
*    - UserInformationService constructor accepts AccountInformation
*    - printAccountInfo() method outputs to stdout
*    - Output format is human-readable and contains key user data
*
* 9. SUCCESS CRITERIA:
*    - All test cases pass
*    - Output contains expected user information
*    - No runtime errors or exceptions
*    - Proper resource cleanup
*/

#include <gtest/gtest.h>
#include "../service/UserInformationService.h"

TEST(UserInfoTest, PrintAccountInfoOutputsCorrectData) {
    AccountInformation acc;
    acc.userName = "Thao";
    acc.password = "123";
    acc.phoneNumber = "0909";
    acc.gmail = "thao@gmail.com";
    acc.role = "user";
    acc.userID = 1; 

    UserInformationService info(acc);


    testing::internal::CaptureStdout();
    info.printAccountInfo();
    std::string output = testing::internal::GetCapturedStdout();


    EXPECT_NE(output.find("Thao"), std::string::npos);              
    EXPECT_NE(output.find("thao@gmail.com"), std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
