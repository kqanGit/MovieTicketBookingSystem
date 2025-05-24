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

