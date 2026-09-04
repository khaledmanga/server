include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(server_test
    channel_test.cpp
    common_test.cpp
    event_loop_test.cpp
    logger_test.cpp
    request_test.cpp
    response_test.cpp
    router_test.cpp
    server_test.cpp
    thread_pool_test.cpp
    ../src/channel.cpp
    ../src/common.cpp
    ../src/event_loop.cpp
    ../src/logger.cpp
    ../src/request.cpp
    ../src/response.cpp
    ../src/router.cpp
    ../src/server.cpp
    ../src/thread_pool.cpp
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
