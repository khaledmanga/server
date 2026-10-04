#include "connection.h"

#include <cerrno>

Connection::Connection(int fd_, const Router &router_, Logger &logger_) : fd(fd_), router(router_), logger(logger_) {}

void Connection::handle_read() {
  read_buffer.resize(4096);
  const ssize_t bytes = recv(fd, read_buffer.data(), read_buffer.size(), 0);
  if (bytes <= 0) {
    return;
  }
  read_buffer.resize(static_cast<std::size_t>(bytes));

  Response response;
  HttpRequestState state = HttpRequestState::Init;
  while (httpParser(request, state, read_buffer) != HttpRequestState::Completed) {
    if (state == HttpRequestState::RequestLine) {
      if (!isValidMethodHttp(request.request_line.method)) {
        response.status(HTTP::StatusCode::NOT_ALLOWED).send("Method Not Allowed");

        this->write_buffer = response.serialize();
        this->handle_write();

        return;
      }
    }
  }

  router.handle(request, response, &logger);

  this->write_buffer = response.serialize();
  this->handle_write();
}

void Connection::handle_write() {
  if (write_buffer.empty()) {
    return;
  }

  for (std::size_t sent = 0; sent < write_buffer.size();) {
    const ssize_t n = send(fd, write_buffer.data() + sent, write_buffer.size() - sent, 0);
    if (n > 0) {
      sent += static_cast<std::size_t>(n);
    } else if (n == 0 || errno != EINTR) {
      return;
    }
  }

  write_buffer.clear();
}