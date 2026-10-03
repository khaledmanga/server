#include "connection.hpp"

#include <cerrno>
#include <iostream>

Connection::Connection(int fd_, const Router &router_, Logger &logger_)
    : fd(fd_), router(router_), logger(logger_) {}

void Connection::handle_read() {
  this->read_buffer.resize(4096);
  const ssize_t bytes =
      recv(this->fd, this->read_buffer.data(), this->read_buffer.size(), 0);

  if (bytes <= 0) {
    return;
  }

  this->read_buffer.resize(static_cast<std::size_t>(bytes));
  HttpRequestState state = HttpRequestState::RequestLine;
  state = httpParser(this->request, state, this->read_buffer);

  Response response;
  if (state == HttpRequestState::Completed) {
    this->router.handle(this->request, response, &this->logger);
  } else {
    response.status(HTTP::StatusCode::BAD_REQUEST).send("Bad Request");
  }

  this->write_buffer = response.serialize();
  std::size_t sent = 0;
  while (sent < this->write_buffer.size()) {
    const ssize_t result = send(this->fd, this->write_buffer.data() + sent,
                                this->write_buffer.size() - sent, 0);
    if (result <= 0) {
      if (result < 0 && errno == EINTR) {
        continue;
      }
      return;
    }
    sent += static_cast<std::size_t>(result);
  }
}
