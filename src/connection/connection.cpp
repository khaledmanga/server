#include "connection.hpp"

Connection::Connection(int socket_fd) : socket_fd(socket_fd) {}

~Connection::~Connection() {
  if (socket_fd != -1) {
    close(socket_fd);
  }
}

void Connection::handle_read() {
  char buffer[1024];
  ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer) - 1);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    read_buffer.append(buffer);
  } else if (bytes_read == 0) {
    close(socket_fd);
    socket_fd = -1;
  } else {
    server_error::perror(server_error::Error{server_error::ErrorCode::READ_FAILED, "Failed to read from socket", errno});
  }
}

void Connection::handle_write() {
  if (!write_buffer.empty()) {
    ssize_t bytes_written = write(socket_fd, write_buffer.c_str(), write_buffer.size());
    if (bytes_written > 0) {
      write_buffer.erase(0, bytes_written);
    } else {
      server_error::perror(server_error::Error{server_error::ErrorCode::WRITE_FAILED, "Failed to write to socket", errno});
    }
  }
}