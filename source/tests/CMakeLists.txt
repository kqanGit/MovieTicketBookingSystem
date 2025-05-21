cmake_minimum_required(VERSION 4.0.2)
project(BookingTest)

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

add_executable(BookingServiceDBTest
    BookingServiceDBTest.cpp
    ../service/BookingService.cpp
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

target_include_directories(BookingServiceDBTest PRIVATE
    ../service
    ../repository
    ../model
    ../database
    ../lib
)

target_link_libraries(BookingServiceDBTest
    gtest
    gmock
    gtest_main
    sqlite3
)

add_executable(MovieViewerServiceDBTest
    MovieViewerServiceDBTest.cpp
    ../service/MovieViewerService.cpp
    ../repository/MovieDTO.cpp
    ../model/Movie.cpp
    ../repository/MovieMapper.cpp
    ../repository/MovieRepositorySQL.cpp
    ../database/DatabaseConnection.cpp
)

target_include_directories(MovieViewerServiceDBTest PRIVATE
    ../service
    ../repository
    ../model
    ../database
    ../lib
)

target_link_libraries(MovieViewerServiceDBTest
    gtest
    gmock
    gtest_main
    sqlite3
)