include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(server_test
    request_test.cpp
    response_test.cpp
    ../src/request.cpp
    ../src/response.cpp
    ../src/router.cpp
)

target_include_directories(server_test
    PRIVATE
        /../src
)

target_link_libraries(server_test
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(server_test)
