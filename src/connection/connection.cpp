#include "connection.hpp"

#include "parser_http/parser_http.hpp"

#include <cerrno>
#include <string>
#include <unistd.h>

namespace {
std::string serialize_response(const Response &response) {
  const std::string content_type =
      response.header.content_type.empty() ? (response.body.type.empty() ? "text/plain" : response.body.type) : response.header.content_type;
  const std::string &body = response.body.content;

  return response.status_line.http_version + " " + std::to_string(response.status_line.status_code) + " " + response.status_line.reason_phrase +
         "\r\nContent-Type: " + content_type + "\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
}
} // namespace

Connection::Connection(int socket_fd) : response(StatusLine("HTTP/1.1", 200, "OK"), Header{}, Body{}), socket_fd(socket_fd) {}

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
  do {
    bytes_read = read(socket_fd, buffer, sizeof(buffer));
  } while (bytes_read < 0 && errno == EINTR);

  if (bytes_read == 0) {
    close(socket_fd);
    socket_fd = -1;
    return;
  }

  if (bytes_read < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return;
    }

    server_error::perror(server_error::Error{server_error::ErrorCode::READ_FAILED, "Failed to read from socket", errno});
    return;
  }

  read_buffer.append(buffer, static_cast<size_t>(bytes_read));
  const HttpParserState state = parse_http_request(read_buffer, request);
  if (state == HttpParserState::COMPLETE) {
    write_buffer = serialize_response(response);
    read_buffer.clear();
  } else if (state == HttpParserState::ERROR) {
    server_error::perror(server_error::Error{server_error::ErrorCode::PARSE_HTTP_FAILED, "Malformed HTTP request", 0});
    close(socket_fd);
    socket_fd = -1;
    read_buffer.clear();
  }
}

void Connection::handle_write() {
  if (socket_fd == -1) {
    return;
  }

  while (!write_buffer.empty()) {
    ssize_t bytes_written;
    do {
      bytes_written = write(socket_fd, write_buffer.data(), write_buffer.size());
    } while (bytes_written < 0 && errno == EINTR);

    if (bytes_written > 0) {
      write_buffer.erase(0, static_cast<size_t>(bytes_written));
      continue;
    }

    if (bytes_written < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return;
    }

    server_error::perror(server_error::Error{server_error::ErrorCode::WRITE_FAILED, "Failed to write to socket", errno});
    return;
  }
}