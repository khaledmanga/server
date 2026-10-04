#pragma once

#include <sys/socket.h>

#include "../http_parser/http_parser.h"
#include "../logger/logger.h"
#include "../response/response.h"
#include "../router/router.h"

class Connection {
 public:
  Connection(int fd_, const Router &router_, Logger &logger_);
  void handle_read();
  void handle_write();

 private:
  int fd;
  const Router &router;
  Logger &logger;
  Request request;
  std::string read_buffer;
  std::string write_buffer;
};
