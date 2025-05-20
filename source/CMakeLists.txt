cmake_minimum_required(VERSION 3.10)

project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 23)

# Thư viện SQLite
add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Thu thập mã nguồn
file(GLOB_RECURSE REPO_SRC "./repository/*.cpp")
file(GLOB_RECURSE DB_SRC "./database/*.cpp")
file(GLOB_RECURSE MODEL_SRC "./model/*.cpp")
file(GLOB_RECURSE USER_SRC "./USER_pending/*.cpp")

# THÊM FILE QUAN TRỌNG TRONG ./source
set(SOURCE_MAIN
    main.cpp
    App.cpp
    SessionManager.cpp
)

add_executable(App
    ${SOURCE_MAIN}
    ${REPO_SRC}
    ${DB_SRC}
    ${MODEL_SRC}
    ${USER_SRC}
)

target_include_directories(App PRIVATE
    ./repository
    ./model
    ./database
    ./lib
    ./USER_pending
    ./ # include App.h, main.cpp, SessionManager.h
)

target_link_libraries(App PRIVATE sqlite3)
