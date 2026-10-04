file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS ../src/*.cc)
list(FILTER SRC_SOURCES EXCLUDE REGEX "main\\.cc$")

add_executable(
  unit_test
  http_parser/http_parser_test.cc
  logger/logger_test.cc
  request/request_test.cc
  response/response_test.cc
  thread_pool/thread_pool_test.cc
  event_loop/event_loop_test.cc
  ${SRC_SOURCES}
)
target_link_libraries(unit_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_test)