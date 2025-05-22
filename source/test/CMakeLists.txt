cmake_minimum_required(VERSION 4.0.1)
project(ServiceTests)

set(CMAKE_CXX_STANDARD 23)

# Thêm GoogleTest từ thư viện phụ
add_subdirectory(../lib/googletest googletest-build)

# Thư viện SQLite tĩnh
add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

# Đặt flags cho file C
target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# === Test: AuthenticationServiceTest ===
add_executable(AuthenticationServiceTest
    AuthenticationServiceTest.cpp
    ../repository/AuthenticationRepositorySQL.cpp
    ../database/DatabaseConnection.cpp
    ../USER_pending/LoginService.cpp
    ../USER_pending/RegisterService.cpp
)

target_include_directories(AuthenticationServiceTest PRIVATE
    ../repository
    ../database
    ../USER_pending
    ../lib
)

target_link_libraries(AuthenticationServiceTest
    gtest
    gmock
    gtest_main
    sqlite3
)

# === Test: SessionAndRoleTest ===
add_executable(SessionAndRoleTest
    SessionAndRoleTest.cpp
    ../SessionManager.cpp
    ../USER_pending/User.cpp
    ../USER_pending/Admin.cpp
    ../USER_pending/Guest.cpp
    ../USER_pending/GuestContextCreator.cpp
    ../USER_pending/UserContextCreator.cpp
    ../USER_pending/AdminContextCreator.cpp
    ../USER_pending/UserInformationService.cpp
)

target_include_directories(SessionAndRoleTest PRIVATE
    ../
    ../USER_pending
    ../lib
)

target_link_libraries(SessionAndRoleTest
    gtest
    gmock
    gtest_main
)

# === Test: VisitorServiceTest ===
add_executable(VisitorServiceTest
    VisitorServiceTest.cpp
    ../USER_pending/LoginServiceVisitor.cpp
    ../USER_pending/RegisterServiceVisitor.cpp
    ../USER_pending/BookingServiceVisitor.cpp
    ../USER_pending/LoginService.cpp
    ../USER_pending/RegisterService.cpp
    ../USER_pending/BookingService.cpp
    ../USER_pending/LogoutService.cpp
    ../USER_pending/UserInformationService.cpp
    ../repository/AuthenticationRepositorySQL.cpp
    ../database/DatabaseConnection.cpp
    ../USER_pending/User.cpp
    ../USER_pending/Admin.cpp
    ../USER_pending/Guest.cpp
    ../USER_pending/GuestContextCreator.cpp
    ../USER_pending/UserContextCreator.cpp
    ../USER_pending/AdminContextCreator.cpp
)

target_include_directories(VisitorServiceTest PRIVATE
    ../USER_pending
    ../repository
    ../database
    ../lib
)

target_link_libraries(VisitorServiceTest
    gtest
    gmock
    gtest_main
    sqlite3
)

# === Test: UserInformationTest ===
add_executable(UserInformationTest
    UserInformationTest.cpp
    ../USER_pending/UserInformationService.cpp
)

target_include_directories(UserInformationTest PRIVATE
    ../USER_pending
    ../lib
)

target_link_libraries(UserInformationTest
    gtest
    gmock
    gtest_main
)

# === Enable CTest integration ===
enable_testing()
add_test(NAME Test_LoginRegister COMMAND AuthenticationServiceTest)
add_test(NAME Test_SessionManager COMMAND SessionAndRoleTest)
add_test(NAME Test_VisitorPattern COMMAND VisitorServiceTest)
add_test(NAME Test_UserInfo COMMAND UserInformationTest)
