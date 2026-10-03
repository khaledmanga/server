file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS ../src/*.cc)
list(FILTER SRC_SOURCES EXCLUDE REGEX "main\\.cc$")

add_executable(
  unit_test
  logger/logger_test.cc
  ${SRC_SOURCES}
)
target_link_libraries(unit_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_test)