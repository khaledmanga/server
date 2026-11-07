#ifndef INCLUDE_ERROR_ERROR_HPP
#define INCLUDE_ERROR_ERROR_HPP

#include <string>
#include <string_view>

namespace server_error {
enum class ErrorCode { SOCKET_CREATION_FAILED, BIND_FAILED, LISTEN_FAILED, ACCEPT_FAILED, READ_FAILED, WRITE_FAILED, PARSE_HTTP_FAILED, INVALID_REQUEST };

struct Error {
  ErrorCode code;
  std::string message;
  int sys_errno;

  Error(ErrorCode code, const std::string &message, int sys_errno) : code(code), message(message), sys_errno(sys_errno) {}
};

std::string_view strerror(ErrorCode code);

void perror(const Error &error);
} // namespace server_error

#endif