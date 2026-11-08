#include "connection.hpp"

#include "parser_http/parser_http.hpp"

#include <cerrno>
#include <string>
#include <unistd.h>

Connection::Connection(int socket_fd, const Router &router)
    : response(StatusLine("HTTP/1.1", 200, "OK"), Header{}, Body{}), socket_fd(socket_fd), router(router) {}

Connection::~Connection() {
  if (socket_fd != -1) {
    close(socket_fd);
  }
}

void Connection::handle_read() {
  if (socket_fd == -1) {
    return;
  }

  char buffer[1024];
  ssize_t bytes_read;
  HttpParserState parser_state = HttpParserState::START;

  bytes_read = read(socket_fd, buffer, sizeof(buffer));

  if (bytes_read < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return;
    }

    server_error::perror(server_error::Error{server_error::ErrorCode::READ_FAILED, "Failed to read from socket", errno});
    return;
  } else if (bytes_read == 0) {
    close(socket_fd);
    socket_fd = -1;
    return;
  }

  this->read_buffer.append(buffer, static_cast<size_t>(bytes_read));

  while (parser_state != HttpParserState::COMPLETE && parser_state != HttpParserState::ERROR) {
    parse_http_request(parser_state, this->read_buffer, this->request);

    if (parser_state == HttpParserState::COMPLETE) {
      break;
    }

    if (parser_state == HttpParserState::ERROR) {
      break;
    }

    bytes_read = read(socket_fd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
      break;
    }

    this->read_buffer.append(buffer, static_cast<size_t>(bytes_read));
  }

  if (parser_state == HttpParserState::ERROR) {
    server_error::perror(server_error::Error{server_error::ErrorCode::INVALID_REQUEST, "Invalid HTTP request", 0});
    return;
  }

  HTTP_METHOD method = from_string(this->request.request_line.method);
  Handler handler;
  Route *route = this->router.match(this->request.request_line.path, method, handler);
  if (route != nullptr) {
    route->handler(this->request, this->response);
    this->write_buffer = this->response.serialize();
  } else {
    this->write_buffer = this->response.not_found();
  }
}

void Connection::handle_write() {
  if (socket_fd == -1) {
    return;
  }

  size_t total_written = 0;
  size_t buffer_size = write_buffer.size();

  while (total_written < buffer_size) {
    ssize_t bytes_written = write(socket_fd, write_buffer.c_str() + total_written, buffer_size - total_written);

    if (bytes_written < 0) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }

      server_error::perror(server_error::Error{server_error::ErrorCode::WRITE_FAILED, "Failed to write to socket", errno});
      return;
    }

    if (bytes_written == 0) {
      break;
    }

    total_written += bytes_written;
  }

  if (total_written > 0) {
    write_buffer.erase(0, total_written);
  }
}

void Connection::run() {
  handle_read();
  handle_write();
}