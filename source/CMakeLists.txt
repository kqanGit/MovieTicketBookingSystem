cmake_minimum_required(VERSION 3.20)
project(MovieTicketBookingSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML paths
set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.2")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")

# Try find SFML with 'main' component
find_package(SFML 2.6 COMPONENTS graphics window system main QUIET)

# If not found, fallback manually
if (NOT SFML_FOUND)
    message(WARNING "SFML not found via find_package. Falling back to manual linking...")

    if (WIN32)
        message(STATUS "Using Windows manual linking")
        # Use debug libraries for debug builds, release for others
        set(SFML_LIBRARIES
            debug "${SFML_LIBRARY_DIR}/sfml-graphics-d.lib"
            debug "${SFML_LIBRARY_DIR}/sfml-window-d.lib"
            debug "${SFML_LIBRARY_DIR}/sfml-system-d.lib"
            debug "${SFML_LIBRARY_DIR}/sfml-main-d.lib"
            optimized "${SFML_LIBRARY_DIR}/sfml-graphics.lib"
            optimized "${SFML_LIBRARY_DIR}/sfml-window.lib"
            optimized "${SFML_LIBRARY_DIR}/sfml-system.lib"
            optimized "${SFML_LIBRARY_DIR}/sfml-main.lib"
        )
    else()
        message(STATUS "Using Unix-like manual linking")
        set(SFML_LIBRARIES
            "${SFML_LIBRARY_DIR}/libsfml-graphics.a"
            "${SFML_LIBRARY_DIR}/libsfml-window.a"
            "${SFML_LIBRARY_DIR}/libsfml-system.a"
            "${SFML_LIBRARY_DIR}/libsfml-main.a"
        )
    endif()
endif()

# Add SQLite static library
add_library(sqlite3 STATIC lib/sqlite3.c)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
)

# Collect source files
file(GLOB_RECURSE REPO_SRC         CONFIGURE_DEPENDS "repository/*.cpp")
file(GLOB_RECURSE DB_SRC           CONFIGURE_DEPENDS "database/*.cpp")
file(GLOB_RECURSE MODEL_SRC        CONFIGURE_DEPENDS "model/*.cpp")
file(GLOB_RECURSE VISITOR_SRC      CONFIGURE_DEPENDS "visitor/*.cpp")
file(GLOB_RECURSE CORE_SRC         CONFIGURE_DEPENDS "core/*.cpp")
file(GLOB_RECURSE CONTEXT_SRC      CONFIGURE_DEPENDS "context/*.cpp")
file(GLOB_RECURSE SERVICE_SRC      CONFIGURE_DEPENDS "service/*.cpp")
file(GLOB_RECURSE UI_SRC           CONFIGURE_DEPENDS "UI/*.cpp")
file(GLOB_RECURSE USER_SRC         CONFIGURE_DEPENDS "USER_pending/*.cpp")

set(MAIN_SRC
    main.cpp
    App.cpp
    SessionManager.cpp
)

add_executable(App
    ${MAIN_SRC}
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

# Include directories
target_include_directories(App PRIVATE
    ${SFML_INCLUDE_DIR}
    ./lib
    ./repository
    ./database
    ./model
    ./visitor
    ./core
    ./context
    ./service
    ./UI

)

# Link libraries
target_link_libraries(App PRIVATE
    sqlite3
)

# Link SFML libraries
if(SFML_FOUND)
    target_link_libraries(App PRIVATE sfml-graphics sfml-window sfml-system sfml-main)
else()
    target_link_libraries(App PRIVATE ${SFML_LIBRARIES})
endif()

# Link Windows system libraries required by SFML
if(WIN32)
    target_link_libraries(App PRIVATE
        opengl32
        winmm
        gdi32
        user32
        advapi32
    )
endif()

# Link Windows system libraries if needed
if(WIN32)
    target_link_libraries(App PRIVATE opengl32 gdi32 winmm)
endif()
