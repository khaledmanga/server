cmake_minimum_required(VERSION 3.17)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "*.cc")

add_executable(server ${SOURCES})

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
