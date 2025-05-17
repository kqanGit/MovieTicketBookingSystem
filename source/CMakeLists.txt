cmake_minimum_required(VERSION 3.16)
project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 17)

# Tìm tất cả file .cpp trong source và các thư mục con
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/*.cpp"
)

# Thêm include path
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/database
    ${CMAKE_SOURCE_DIR}/repository
    ${CMAKE_SOURCE_DIR}/USER_pending
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/model
)

add_executable(main ${SOURCES})
