#include "connection.h"

#include "http/response/response.h"
#include <iostream>
#include <string>
#include <unistd.h>

Connection::Connection(int client_fd) { this->client_fd = client_fd; }

bool Connection::handle_read() {
  HttpStatus http_status = HttpStatus::RequestLine;
  char buffer[4096];

  while (http_status != HttpStatus::End) {
    const ssize_t bytes_read = read(this->client_fd, buffer, 4096);
    if (bytes_read <= 0) {
      return false;
    }

    this->read_buffer.append(buffer, (size_t)(bytes_read));
    http_parser(this->request, http_status, this->read_buffer);

    if (http_status == HttpStatus::Error) {
      return false;
    }
  }

  return true;
}

void Connection::handle_write() {
  Response response;
  response.body.value = "OK\n";
  response.header.kv["Content-Length"] = std::to_string(response.body.value.size());
  response.header.kv["Content-Type"] = "text/plain";

  const std::string output = response.ok();
  for (size_t sent = 0; sent < output.size();) {
    const ssize_t n = write(client_fd, output.data() + sent, output.size() - sent);
    if (n <= 0) {
      std::cerr << "Failed to write HTTP response\n";
      return;
    }
    sent += (size_t)(n);
  }
}

void Connection::run() {
  if (this->handle_read()) {
    this->handle_write();
  }
  close(this->client_fd);
}