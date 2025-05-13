cmake_minimum_required(VERSION 4.0.2)
project(BookingRepositoryTest)

set(CMAKE_CXX_STANDARD 23)

# Thêm googletest submodule (đường dẫn tương đối từ tests/)
add_subdirectory(../lib/googletest googletest-build)

# Thêm file sqlite3.c vào project
add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

# Đặt flags cho file C
target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Tạo executable cho file test với database mới từ script SQL
add_executable(BookingRepositoryDBTest
    BookingRepositoryDBTest.cpp
    ../repository/BookingRepositorySQL.cpp
    ../repository/BookingView.cpp
    ../repository/SeatView.cpp
    ../database/DatabaseConnection.cpp
    ../model/Booking.cpp
    ../model/ShowTime.cpp
    ../model/SingleSeat.cpp
    ../model/CoupleSeat.cpp
    ../model/Movie.cpp
)

# Thêm include path cho test DB
target_include_directories(BookingRepositoryDBTest PRIVATE
    ../repository
    ../model
    ../database
    ../lib
)

# Link Google Test và Google Mock cho DB test
target_link_libraries(BookingRepositoryDBTest
    gtest
    gmock
    gtest_main
    sqlite3
)