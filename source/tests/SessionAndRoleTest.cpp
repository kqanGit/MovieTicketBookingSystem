
/*
* TEST PLAN FOR SESSION MANAGER AND ROLE MANAGEMENT
* =================================================
*
* 1. PURPOSE:
*    - Verify SessionManager functionality for user context management
*    - Test role-based access control and context switching
*    - Ensure proper user session lifecycle management
*    - Validate context creation for different user types (Guest, User, Admin)
*
* 2. TEST SCOPE:
*    - Session initialization and context management
*    - Role-based service access validation
*    - User context creation and switching
*    - Guest mode default behavior
*    - Authentication state management
*
* 3. TEST CASES:
*    3.1. GuestHasNoUserInfoService:
*         - Description: Verify Guest context has limited access to services
*         - Expected Behavior: 
*           + Guest context should not have UserInformationService
*           + getCurrentRole() should return "Guest"
*         - Security Implication: Guests cannot access user-specific information
*
*    3.2. UserHasCorrectRoleInfo:
*         - Description: Test User context creation and role assignment
*         - Input: AccountInformation with role "User"
*         - Expected Behavior:
*           + setUserContext() returns true for valid user
*           + Context has valid UserInformationService
*           + Service returns correct role "User"
*           + SessionManager reports correct role
*
*    3.3. AdminHasCorrectRoleInfo:
*         - Description: Test Admin context creation and role assignment
*         - Input: AccountInformation with role "Admin"
*         - Expected Behavior:
*           + setUserContext() returns true for valid admin
*           + Context has valid UserInformationService
*           + Service returns correct role "Admin"
*           + SessionManager reports correct role
*
* 4. FUTURE TEST CASES (TO BE IMPLEMENTED):
*    4.1. SessionLogoutReturnsToGuest:
*         - Test logout functionality returns to Guest context
*    4.2. InvalidUserContextCreationFails:
*         - Test behavior with invalid/corrupted account information
*    4.3. ConcurrentSessionManagement:
*         - Test thread safety (if applicable)
*    4.4. SessionPersistenceTest:
*         - Test session state persistence across operations
*    4.5. RoleEscalationPrevention:
*         - Test that users cannot escalate to admin privileges
*    4.6. ContextSwitchingValidation:
*         - Test switching between different user contexts
*
* 5. SECURITY TESTING:
*    - Role-based access control enforcement
*    - Prevention of unauthorized service access
*    - Proper context isolation between user types
*    - Secure session state management
*
* 6. TEST DATA:
*    - User Account: "user01", password "pass", phone "012", email "user01@gmail.com", role "User"
*    - Admin Account: "admin01", password "pass", phone "099", email "admin@gmail.com", role "Admin"
*    - Guest: Default context (no account information)
*
* 7. TESTING METHODOLOGY:
*    - Use ASSERT_* for critical preconditions
*    - Use EXPECT_* for result validation
*    - Test both positive and negative scenarios
*    - Verify null pointer safety
*    - Check role string consistency
*
* 8. DEPENDENCIES:
*    - SessionManager class
*    - Guest, User, Admin model classes
*    - UserInformationService
*    - AccountInformation struct
*    - Context creation pattern implementation
*
* 9. DESIGN PATTERNS TESTED:
*    - Context Pattern: Different user contexts
*    - Factory Pattern: Context creation based on user type
*    - Strategy Pattern: Role-based service access
*
* 10. SUCCESS CRITERIA:
*     - All role transitions work correctly
*     - Service access matches role permissions
*     - No memory leaks or resource issues
*     - Secure role isolation maintained
*     - Consistent behavior across context switches
*/

#include <gtest/gtest.h>
#include "../SessionManager.h"
#include "../model/Guest.h"
#include "../model/User.h"
#include "../model/Admin.h"
#include "../service/UserInformationService.h"
#include "../service/LogoutService.h"
#include "../service/ILogoutService.h"

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
    ASSERT_TRUE(manager.setUserContext(acc));    auto info = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(info, nullptr);
    EXPECT_EQ(info->getRole(), "Admin");
    EXPECT_EQ(manager.getCurrentRole(), "Admin");
}

// Test logout functionality returns to Guest context
TEST(SessionManagerTest, SessionLogoutReturnsToGuest) {
    AccountInformation userAcc = { "user01", "pass", "012", "user01@gmail.com", "User" };
      SessionManager manager;
    std::shared_ptr<ILogoutService> service = std::make_shared<LogoutService>();
    
    // Initially should be Guest
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
    EXPECT_FALSE(manager.isUserAuthenticated());
    
    // Login as User
    ASSERT_TRUE(manager.setUserContext(userAcc));
    EXPECT_EQ(manager.getCurrentRole(), "User");
    EXPECT_TRUE(manager.isUserAuthenticated());
    
    // Verify user has UserInformationService
    auto userInfo = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(userInfo, nullptr);
    EXPECT_EQ(userInfo->getRole(), "User");
    
    // Logout should return to Guest context
    ASSERT_TRUE(manager.logout(service));
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
    EXPECT_FALSE(manager.isUserAuthenticated());
    
    // Guest should not have UserInformationService
    auto guestInfo = manager.getCurrentContext()->getUserInformationService();
    EXPECT_EQ(guestInfo, nullptr);
}

// Test switching between different user contexts
TEST(SessionManagerTest, ContextSwitchingValidation) {
    AccountInformation userAcc = { "user01", "pass", "012", "user01@gmail.com", "User" };
    AccountInformation adminAcc = { "admin01", "pass", "099", "admin@gmail.com", "Admin" };
    
    SessionManager manager;
    std::shared_ptr<ILogoutService> service = std::make_shared<LogoutService>();
    
    // Start as Guest
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
    EXPECT_FALSE(manager.isUserAuthenticated());
    
    // Switch to User context
    ASSERT_TRUE(manager.setUserContext(userAcc));
    EXPECT_EQ(manager.getCurrentRole(), "User");
    EXPECT_TRUE(manager.isUserAuthenticated());
      // Verify User context properties
    auto userInfo = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(userInfo, nullptr);
    EXPECT_EQ(userInfo->getRole(), "User");
    EXPECT_EQ(manager.getCurrentAccount().userName, "user01");
    EXPECT_EQ(manager.getCurrentAccount().gmail, "user01@gmail.com");
    
    // Logout to Guest
    ASSERT_TRUE(manager.logout(service));
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
    EXPECT_FALSE(manager.isUserAuthenticated());
    
    // Switch to Admin context
    ASSERT_TRUE(manager.setUserContext(adminAcc));
    EXPECT_EQ(manager.getCurrentRole(), "Admin");
    EXPECT_TRUE(manager.isUserAuthenticated());
      // Verify Admin context properties
    auto adminInfo = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(adminInfo, nullptr);
    EXPECT_EQ(adminInfo->getRole(), "Admin");
    EXPECT_EQ(manager.getCurrentAccount().userName, "admin01");
    EXPECT_EQ(manager.getCurrentAccount().gmail, "admin@gmail.com");
    
    // Logout back to Guest
    ASSERT_TRUE(manager.logout(service));
    EXPECT_EQ(manager.getCurrentRole(), "Guest");
    EXPECT_FALSE(manager.isUserAuthenticated());
    
    // Verify Guest context after multiple switches
    auto finalGuestInfo = manager.getCurrentContext()->getUserInformationService();
    EXPECT_EQ(finalGuestInfo, nullptr);
      // Test direct context switching (User -> Admin without logout)
    ASSERT_TRUE(manager.setUserContext(userAcc));
    EXPECT_EQ(manager.getCurrentRole(), "User");
    
    // Switch directly to Admin (should FAIL - direct switching not allowed)
    ASSERT_FALSE(manager.setUserContext(adminAcc));
    EXPECT_EQ(manager.getCurrentRole(), "User");  // Should remain User
    EXPECT_TRUE(manager.isUserAuthenticated());
    
    // Verify the context was NOT switched - should still be User
    auto switchedUserInfo = manager.getCurrentContext()->getUserInformationService();
    ASSERT_NE(switchedUserInfo, nullptr);
    EXPECT_EQ(switchedUserInfo->getRole(), "User");  // Should still be User
    EXPECT_EQ(manager.getCurrentAccount().userName, "user01");  // Should still be user01
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}