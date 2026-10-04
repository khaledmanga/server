file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cc)

add_executable(server ${SOURCES})

target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)