cmake_minimum_required(VERSION 3.16)
project(DatabaseProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put the compiled binary directly in build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Include headers from include/
include_directories(include)

# Collect all source files
add_executable(db_engine
    src/main.cpp
    src/Database.cpp
    src/Page.cpp
    src/PageManager.cpp
)