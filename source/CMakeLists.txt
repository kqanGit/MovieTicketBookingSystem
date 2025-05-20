cmake_minimum_required(VERSION 4.0.1)

set(CMAKE_CXX_STANDARD 23)

add_library(sqlite3 STATIC
    ../lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Lấy tất cả file cpp trong các thư mục
file(GLOB REPO_SRC "./repository/*.cpp")
file(GLOB DB_SRC "./database/*.cpp")
file(GLOB MODEL_SRC "./model/*.cpp")
file(GLOB USER_SRC "./USER_pending/*.cpp")

add_executable(App
    App.cpp
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
)

target_link_libraries(App PRIVATE sqlite3)
