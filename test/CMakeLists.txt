include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(server_test
    request_test.cpp
    ../src/request.cpp
)

target_include_directories(server_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(server_test
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(server_test)
