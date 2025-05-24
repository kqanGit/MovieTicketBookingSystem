cmake_minimum_required(VERSION 4.0.2)
project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 23)

# SQLite library
add_library(sqlite3 STATIC
    lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Collect source files
file(GLOB_RECURSE REPO_SRC "./repository/*.cpp")
file(GLOB_RECURSE DB_SRC "./database/*.cpp")
file(GLOB_RECURSE MODEL_SRC "./model/*.cpp")
file(GLOB_RECURSE VISITOR_SRC "./visitor/*.cpp")
file(GLOB_RECURSE CORE_SRC "./core/*.cpp")
file(GLOB_RECURSE CONTEXT_SRC "./context/*.cpp")
file(GLOB_RECURSE SERVICE_SRC "./service/*.cpp")
file(GLOB_RECURSE UI_SRC "./UI/*.cpp")
file(GLOB_RECURSE USER_SRC "./USER_pending/*.cpp")

# Main source files
set(SOURCE_MAIN
    main.cpp
    SessionManager.cpp
    App.cpp
)

add_executable(App
    ${SOURCE_MAIN}
    ${REPO_SRC}
    ${DB_SRC}
    ${MODEL_SRC}
    ${VISITOR_SRC}
    ${CORE_SRC}
    ${CONTEXT_SRC}
    ${SERVICE_SRC}
    ${UI_SRC}
    ${USER_SRC}
)

target_include_directories(App PRIVATE
    ./
    ./lib
    ./repository
    ./model
    ./database
    ./visitor
    ./context
    ./core
    ./service
    ./UI
    ./USER_pending
)
