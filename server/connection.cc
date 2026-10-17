#include "connection.h"

#include "http/response/response.h"
#include <iostream>
#include <string>
#include <unistd.h>

Connection::Connection(int client_fd, Router router) : client_fd(client_fd), router(std::move(router)) {}

void Connection::handle_read() {
  char buffer[4096];
  const ssize_t bytes_read = read(this->client_fd, buffer, sizeof(buffer));
  if (bytes_read > 0) {
    this->read_buffer.append(buffer, static_cast<size_t>(bytes_read));
  }

  while (this->parser_status != HttpStatus::End && this->parser_status != HttpStatus::Error) {
    const size_t previous_size = this->read_buffer.size();
    http_parser(this->request, this->parser_status, this->read_buffer);
    if (previous_size == this->read_buffer.size()) {
      break;
    }
  }
}

void Connection::handle_router() {
  Route *route = this->router.match(this->request.request_line.path);
  if (route) {
    route->handle(this->request, this->response);
  } else {
    this->response.status_code = StatusCode::NotFound;
    this->response.body.value = "Not found";
  }
  this->write_buffer = this->response.serialize();
}

void Connection::handle_write() {
  if (this->write_buffer.empty()) {
    return;
  }

  const ssize_t sent = write(this->client_fd, this->write_buffer.data(), this->write_buffer.size());
  if (sent > 0) {
    this->write_buffer.erase(0, static_cast<size_t>(sent));
  }
}