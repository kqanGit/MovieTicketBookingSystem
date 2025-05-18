# CMake configuration for MovieTicketBookingSystem
cmake_minimum_required(VERSION 4.0.2)
project(app)

set(CMAKE_CXX_STANDARD 23)

# SQLite library
add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Main application
file(GLOB REPO_SRC ./repository/*.cpp)
file(GLOB DB_SRC ./database/*.cpp)
file(GLOB MODEL_SRC ./model/*.cpp)
file(GLOB USER_PENDING_SRC ./USER_pending/*.cpp)

# Tạo executable từ các file nguồn
add_executable(App
    App.cpp
    SessionManager.cpp
    main.cpp
    ${REPO_SRC}
    ${DB_SRC}
    ${MODEL_SRC}
    ${USER_PENDING_SRC}
)

target_include_directories(App PRIVATE
    ./repository
    ./model
    ./database
    ./lib
    ./USER_pending
    .
)

# Link with SQLite
target_link_libraries(App PRIVATE sqlite3)
