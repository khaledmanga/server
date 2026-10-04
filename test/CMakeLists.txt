file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cc)
file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)
list(FILTER SRC_SOURCES EXCLUDE REGEX "/main\\.cc$")

add_executable(unit_test ${TEST_SOURCES} ${SRC_SOURCES})
target_link_libraries(unit_test PRIVATE GTest::gtest_main nlohmann_json::nlohmann_json)

include(GoogleTest)
gtest_discover_tests(unit_test)