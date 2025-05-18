cmake_minimum_required(VERSION 3.10)
project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 17)

# Chỉ định rõ các file .cpp cần build, tránh lấy CMakeCXXCompilerId.cpp
file(GLOB MAIN_SOURCES
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/App.cpp"
    "${CMAKE_SOURCE_DIR}/SessionManager.cpp"
)

file(GLOB DATABASE_SOURCES "${CMAKE_SOURCE_DIR}/database/*.cpp")
file(GLOB MODEL_SOURCES "${CMAKE_SOURCE_DIR}/model/*.cpp")
file(GLOB REPO_SOURCES "${CMAKE_SOURCE_DIR}/repository/*.cpp") 
file(GLOB USER_SOURCES "${CMAKE_SOURCE_DIR}/USER_pending/*.cpp")

# Thêm SQLite
set(SQLITE_SOURCES
    ${CMAKE_SOURCE_DIR}/lib/sqlite3.c
)

# Thêm include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/database
    ${CMAKE_SOURCE_DIR}/repository
    ${CMAKE_SOURCE_DIR}/USER_pending
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/model
)

# Tạo executable
add_executable(main 
    ${MAIN_SOURCES} 
    ${DATABASE_SOURCES}
    ${MODEL_SOURCES}
    ${REPO_SOURCES}
    ${USER_SOURCES}
    ${SQLITE_SOURCES}
)

# Liên kết các thư viện cần thiết
if(WIN32)
    target_link_libraries(main PRIVATE)
else()
    target_link_libraries(main pthread dl)
endif()
