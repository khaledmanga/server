file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) 
FetchContent_MakeAvailable(googletest)

add_executable(server_unit_tests ${TEST_SOURCES})

target_include_directories(server_unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(server_unit_tests PRIVATE server_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(server_unit_tests)
