#ifndef CONNECTION_CONNECTION_HPP
#define CONNECTION_CONNECTION_HPP

#include "http/request/request.hpp"
#include "http/response/response.hpp"
#include "include/error/error.hpp"
#include "router/router.hpp"
#include <string>

class Connection {
public:
  Connection(int socket_fd, const Router& router);
  ~Connection();

  Request request;
  Response response;
  const Router &router;

  std::string read_buffer;
  std::string write_buffer;
  int socket_fd;

  void handle_read();
  void handle_write();
  void run();
};

#endif