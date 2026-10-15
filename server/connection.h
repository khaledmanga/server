#pragma once

#include <string>

#include "http/http_parser/http_parser.h"
#include "http/request/request.h"
#include "include/common.h"

class Connection {
private:
  int client_fd;
  std::string read_buffer;
  Request request;
  bool handle_read();
  void handle_write();

public:
  Connection(int);
  void run();
};
