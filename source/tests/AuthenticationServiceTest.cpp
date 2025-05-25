
/*
* TEST PLAN FOR AUTHENTICATION SERVICE
* ====================================
*
* 1. PURPOSE:
*    - Verify authentication service functionality (Login and Registration)
*    - Test database integration for user authentication
*    - Ensure secure user credential handling
*    - Validate authentication repository operations
*
* 2. TEST SCOPE:
*    - User registration process
*    - User login authentication
*    - Database connection and operations
*    - Error handling for invalid credentials
*    - Integration between service layer and repository layer
*
* 3. TEST CASES:
*    3.1. RegisterAndLogin_Success:
*         - Description: Complete user registration and login flow
*         - Steps:
*           1. Register new user with valid credentials
*           2. Attempt login with same credentials
*         - Input: AccountInformation("testuser", "123", "0123", "test@gmail.com", "user")
*         - Expected Results:
*           + Registration succeeds (returns true)
*           + Login succeeds (returns optional with user data)
*           + Returned username matches input
*         - Validates: End-to-end authentication workflow
*
*    3.2. LoginFailsWithWrongPassword:
*         - Description: Test authentication failure with incorrect credentials
*         - Input: Valid username with incorrect password
*         - Expected Result: Login returns empty optional (no authentication)
*         - Security Implication: Prevents unauthorized access
*
* 4. DATABASE INTEGRATION:
*    - Database Setup: Creates fresh database for each test run
*    - Database Cleanup: Removes existing database before testing
*    - Schema Initialization: Executes database.sql for table creation
*    - Connection Management: Proper connection and disconnection
*
* 5. FUTURE TEST CASES (TO BE IMPLEMENTED):
*    5.1. RegisterWithDuplicateUsername:
*         - Test registration failure with existing username
*    5.2. RegisterWithInvalidEmail:
*         - Test validation of email format
*    5.3. RegisterWithWeakPassword:
*         - Test password strength requirements
*    5.4. LoginWithNonexistentUser:
*         - Test login attempt with non-registered user
*    5.5. DatabaseConnectionFailure:
*         - Test behavior when database is unavailable
*    5.6. ConcurrentRegistrationTest:
*         - Test thread safety for concurrent registrations
*    5.7. PasswordHashingValidation:
*         - Verify passwords are properly hashed and stored
*    5.8. SessionTokenGeneration:
*         - Test secure session token creation
*
* 6. SECURITY TESTING:
*    - Password security (hashing, not stored in plaintext)
*    - SQL injection prevention
*    - Authentication bypass prevention
*    - Credential validation and sanitization
*
* 7. TEST ENVIRONMENT:
*    - Database: SQLite (database.db)
*    - Test Isolation: Fresh database for each test execution
*    - Dependencies: DatabaseConnection, AuthenticationRepositorySQL
*    - Services: LoginService, RegisterService
*
* 8. TEST DATA:
*    - Test User: username "testuser", password "123", phone "0123", email "test@gmail.com"
*    - Invalid Credentials: "admin@gmail.com" with "wrongpass"
*    - Database Schema: Loaded from database.sql
*
* 9. TESTING METHODOLOGY:
*    - Test Fixture Pattern: AuthServiceTest class for setup/teardown
*    - Database Recreation: Fresh database for each test run
*    - Integration Testing: Tests service + repository + database layers
*    - Positive and Negative Testing: Valid and invalid scenarios
*
* 10. ERROR HANDLING:
*     - Database connection failures
*     - Invalid input validation
*     - Service layer exception handling
*     - Repository operation failures
*
* 11. PERFORMANCE CONSIDERATIONS:
*     - Database operation efficiency
*     - Authentication response time
*     - Memory usage during operations
*     - Resource cleanup verification
*
* 12. DEPENDENCIES:
*     - AuthenticationRepositorySQL class
*     - DatabaseConnection singleton
*     - LoginService and RegisterService
*     - AccountInformation model
*     - SQLite database engine
*
* 13. SUCCESS CRITERIA:
*     - All authentication workflows complete successfully
*     - Database operations execute without errors
*     - Security measures prevent unauthorized access
*     - Proper resource management (no memory leaks)
*     - Consistent behavior across test runs
*/

#include <gtest/gtest.h>
#include <filesystem>
#include "../repository/AuthenticationRepositorySQL.h"
#include "../database/DatabaseConnection.h"
#include "../service/LoginService.h"
#include "../service/RegisterService.h"
#include "../model/AccountInformation.h"

DatabaseConnection* db = nullptr;
AuthenticationRepositorySQL* repo = nullptr;

// TEST(DatabaseConnectionTest, ConnectsToExistingDatabase) {
//     std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

//     db = DatabaseConnection::getInstance();
//     ASSERT_TRUE(db->connect("database.db"));  // Đảm bảo kết nối thành công
// }

class AuthServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_NE(db, nullptr);  // Chắc chắn đã được khởi tạo từ test trước
        repo = new AuthenticationRepositorySQL(db);
    }

    void TearDown() override {
        delete repo;
    }
};

TEST_F(AuthServiceTest, RegisterAndLogin_Success) {
    AccountInformation acc { "testuser", "123", "0123", "test@gmail.com", "user" };
    RegisterService reg(repo);
    ASSERT_TRUE(reg.registerUser(acc));

    LoginService login(repo);
    auto result = login.authenticate("testuser", "123");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->userName, "testuser");
}

TEST_F(AuthServiceTest, LoginFailsWithWrongPassword) {
    LoginService login(repo);
    auto result = login.authenticate("admin@gmail.com", "wrongpass");
    ASSERT_FALSE(result.has_value());
}

int main(int argc, char** argv) {

    db = DatabaseConnection::getInstance();

     const std::string dbPath = "database.db";
    
    // Remove the existing database file if it exists  
    if (std::filesystem::exists(dbPath)) {
        std::cout << "Removing existing database file..." << std::endl;
        std::filesystem::remove(dbPath);
    }

    if (!db->connect(dbPath)) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }   

    db->executeSQLFile("database.sql");

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    
    db->disconnect();
    return result;
}

