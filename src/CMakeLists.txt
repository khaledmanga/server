file(GLOB_RECURSE SERVER_CORE_SOURCES CONFIGURE_DEPENDS "*.cpp")
list(FILTER SERVER_CORE_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_library(server_core ${SERVER_CORE_SOURCES})
target_include_directories(server_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(server_bin main.cpp)
target_link_libraries(server_bin PRIVATE server_core)
