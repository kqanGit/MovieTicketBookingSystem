cmake_minimum_required(VERSION 4.0.1)
project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True) # Ensure the standard is enforced

# SQLite library
add_library(sqlite3 STATIC
    lib/sqlite3.c
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Collect source files
file(GLOB_RECURSE REPO_SRC CONFIGURE_DEPENDS "./repository/*.cpp")
file(GLOB_RECURSE DB_SRC CONFIGURE_DEPENDS "./database/*.cpp")
file(GLOB_RECURSE MODEL_SRC CONFIGURE_DEPENDS "./model/*.cpp")
file(GLOB_RECURSE VISITOR_SRC CONFIGURE_DEPENDS "./visitor/*.cpp")
file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS "./core/*.cpp") # Will be empty if no .cpp files
file(GLOB_RECURSE CONTEXT_SRC CONFIGURE_DEPENDS "./context/*.cpp")
file(GLOB_RECURSE SERVICE_SRC CONFIGURE_DEPENDS "./service/*.cpp")
file(GLOB_RECURSE UI_SRC CONFIGURE_DEPENDS "./UI/*.cpp") # Will be empty if no .cpp files
file(GLOB_RECURSE USER_SRC CONFIGURE_DEPENDS "./USER_pending/*.cpp") # Will be empty if no .cpp files

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
    ./ # Root of the source directory
    ./lib # For sqlite3.h
    # Add other necessary include directories if direct subfolder includes are used
    # e.g., #include "Movie.h" instead of #include "model/Movie.h"
    # If all includes are relative to source root (e.g. "model/Movie.h"), 
    # then only "./" might be strictly needed here, but listing them doesn't hurt.
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

# Link libraries
target_link_libraries(App PRIVATE sqlite3)

# Optional: Add compiler flags if needed, for example for warnings
# if(MSVC)
#   target_compile_options(App PRIVATE /W4)
# else()
#   target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
# endif()
