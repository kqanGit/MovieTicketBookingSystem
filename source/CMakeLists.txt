cmake_minimum_required(VERSION 4.0.2)

set(CMAKE_CXX_STANDARD 23)

add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

add_executable(App
    App.cpp
    ./repository/*.cpp
    ./database/*.cpp
    ./model/*.cpp
    ./USER_pending/*.cpp
)

target_include_directories(App PRIVATE
    ./repository
    ./model
    ./database
    ./lib
    ./USER_pending
)
