#include "http/request/request.hpp"
#include "http/response/response.hpp"
#include "http_request/request.hpp"
#include "include/error/error.hpp"
#include <string>

class Connection {
public:
  Connection(int socket_fd);
  ~Connection();

  Request request;
  Response response;

  std::string read_buffer;
  std::string write_buffer;
  int socket_fd;

  void handle_read();
  void handle_write();
}