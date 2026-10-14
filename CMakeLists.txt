cmake_minimum_required(VERSION 3.17)

project(server)

add_executable(server
    main.cpp
    include/common.cpp
    http/http_parser/http_parser.cc
    http/request/request.cc
    http/response/response.cc
    server/connection.cc
    server/server.cc
)

target_include_directories(server PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")