#include "error.hpp"

#include <iostream>
#include <string>

namespace server_error {
std::string_view strerror(ErrorCode code) {
  switch (code) {
  case ErrorCode::SOCKET_CREATION_FAILED:
    return "Socket creation failed";
  case ErrorCode::BIND_FAILED:
    return "Bind failed";
  case ErrorCode::LISTEN_FAILED:
    return "Listen failed";
  case ErrorCode::ACCEPT_FAILED:
    return "Accept failed";
  case ErrorCode::READ_FAILED:
    return "Read failed";
  case ErrorCode::WRITE_FAILED:
    return "Write failed";
  case ErrorCode::PARSE_HTTP_FAILED:
    return "HTTP parsing failed";
  default:
    return "Unknown error";
  }
}

void perror(const Error &error) { std::cerr << "[Error " << error.sys_errno << "]: " << strerror(error.code) << ": " << error.message << std::endl; }
} // namespace server_error