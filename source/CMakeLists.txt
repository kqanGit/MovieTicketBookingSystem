cmake_minimum_required(VERSION 3.10)

project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 23)

# Thư viện SQLite
add_library(sqlite3 STATIC
    lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Thu thập mã nguồn
file(GLOB_RECURSE REPO_SRC "./repository/*.cpp")
file(GLOB_RECURSE DB_SRC "./database/*.cpp")
file(GLOB_RECURSE MODEL_SRC "./model/*.cpp")
file(GLOB_RECURSE VISITOR_SRC "./visitor/*.cpp")
file(GLOB_RECURSE CORE_SRC "./core/*.cpp")
file(GLOB_RECURSE CONTEXT_SRC "./context/*.cpp")
file(GLOB_RECURSE SERVICE_SRC "./service/*.cpp")
file(GLOB_RECURSE UI_SRC "./UI/*.cpp")

# THÊM FILE QUAN TRỌNG TRONG ./source
set(SOURCE_MAIN
    main.cpp
    SessionManager.cpp
    App.cpp # Add App.cpp
)

add_executable(App
    ${SOURCE_MAIN}
    ${REPO_SRC}
    ${DB_SRC}
    ${MODEL_SRC}
    ${CONTEXT_SRC}  # Add CONTEXT_SRC
    ${CORE_SRC}     # Add CORE_SRC
    ${SERVICE_SRC}  # Add SERVICE_SRC
    ${UI_SRC}       # Add UI_SRC
    ${VISITOR_SRC}  # Add VISITOR_SRC
    ${USER_SRC}     # USER_SRC is now defined and included
)

target_include_directories(App PRIVATE
    ./ # For App.h, main.cpp, SessionManager.h, etc. in source/
    ./lib
    ./repository
    ./model
    ./database
    ./USER_pending
    ./context   # Add include path for context
    ./core      # Add include path for core
    ./service   # Add include path for service
    ./UI        # Add include path for UI
    ./visitor   # Add include path for visitor
)

target_link_libraries(App PRIVATE sqlite3) # Link App against sqlite3
