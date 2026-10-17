#pragma once

#include <string>

#include "http/http_parser/http_parser.h"
#include "http/request/request.h"
#include "include/common.h"
#include "router/router.h"

class Connection {
  friend class Server;

private:
  int client_fd;
  std::string read_buffer;
  std::string write_buffer;
  Request request;
  Response response;
  Router router;
  HttpStatus parser_status = HttpStatus::RequestLine;
  void handle_read();
  void handle_write();
  void handle_router();

public:
  Connection(int, Router);
  void run();
};
