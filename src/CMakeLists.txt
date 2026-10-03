file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cc)

add_executable(server ${SOURCES})