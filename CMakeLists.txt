cmake_minimum_required(VERSION 3.16)
project(DatabaseProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include)

add_executable(db_engine
    src/main.cpp
    src/Page.cpp
    src/PageManager.cpp
    src/DiskManager.cpp
    src/BufferPool.cpp
)