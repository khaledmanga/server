#pragma once

#include <sys/socket.h>

#include "../http_parser/http_parser.hpp"
#include "../logger/logger.hpp"
#include "../response/response.hpp"
#include "../router/router.hpp"

class Connection {
 public:
  Connection(int fd_, const Router &router_, Logger &logger_);
  void handle_read();

 private:
  int fd;
  const Router &router;
  Logger &logger;
  Request request;
  std::string read_buffer;
  std::string write_buffer;
};
