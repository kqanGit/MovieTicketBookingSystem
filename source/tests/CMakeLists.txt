cmake_minimum_required(VERSION 4.0.2)
project(MovieTicketBookingSystemTests)


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../lib/googletest googletest-build)


add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

file(GLOB_RECURSE REPO_SRC "../repository/*.cpp")
file(GLOB_RECURSE DB_SRC "../database/*.cpp")
file(GLOB_RECURSE MODEL_SRC "../model/*.cpp")
file(GLOB_RECURSE VISITOR_SRC "../visitor/*.cpp")
file(GLOB_RECURSE CORE_SRC "../core/*.cpp")
file(GLOB_RECURSE CONTEXT_SRC "../context/*.cpp")
file(GLOB_RECURSE SERVICE_SRC "../service/*.cpp")


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
    ../model/ShowTime.cpp
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

#################################################################################

add_executable(AuthenticationServiceTest
    AuthenticationServiceTest.cpp
    ../service/LoginService.cpp
    ../service/RegisterService.cpp
    ../repository/AuthenticationRepositorySQL.cpp
    ../database/DatabaseConnection.cpp
)

target_include_directories(AuthenticationServiceTest PRIVATE
    ../service
    ../repository
    ../model
    ../database
    ../lib
)

target_link_libraries(AuthenticationServiceTest
    gtest
    gmock
    gtest_main
    sqlite3
)

###################################################################

add_executable(SessionAndRoleTest
    SessionAndRoleTest.cpp
    ../model/User.cpp
    ../model/Admin.cpp
    ../model/Guest.cpp
    ../SessionManager.cpp
    ../service/UserInformationService.cpp
    ../service/LogoutService.cpp
    ${CONTEXT_SRC}
    ${VISITOR_SRC}
)

target_include_directories(SessionAndRoleTest PRIVATE
    ../service
    ../model
    ../context
    ../visitor
    ../core
)

target_link_libraries(SessionAndRoleTest
    gtest
    gmock
    gtest_main
    sqlite3
)

###########################################################

add_executable(UserInformationTest
    UserInformationTest.cpp
    ../service/UserInformationService.cpp
)

target_include_directories(UserInformationTest PRIVATE
    ../service
    ../model
)

target_link_libraries(UserInformationTest
    gtest
    gmock
    gtest_main
    sqlite3
)

###########################################################


add_executable(VisitorServiceTest
    VisitorServiceTest.cpp
    ${REPO_SRC}
    ${DB_SRC}
    ${MODEL_SRC}
    ${VISITOR_SRC}
    ${CORE_SRC}
    ${CONTEXT_SRC}
    ${SERVICE_SRC}
)

target_include_directories(VisitorServiceTest PRIVATE
    ../
    ../lib
    ../repository
    ../model
    ../database
    ../visitor
    ../context
    ../core
    ../service
)

target_link_libraries(VisitorServiceTest
    gtest
    gmock
    gtest_main
    sqlite3
)
