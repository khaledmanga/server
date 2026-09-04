#include "server.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int port)
    : port_(port) {
  setupSocket();
}

Server::~Server() {
  if (server_fd_ >= 0) {
    close(server_fd_);
  }
}

void Server::setupSocket() {
  server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ < 0) {
    throw std::runtime_error("Failed to create socket");
  }

  int option = 1;
  setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd_, reinterpret_cast<sockaddr *>(&server_address),
           sizeof(server_address)) < 0) {
    close(server_fd_);
    server_fd_ = -1;
    throw std::runtime_error("Failed to bind");
  }

  if (listen(server_fd_, SOMAXCONN) < 0) {
    throw std::runtime_error("Failed to listen");
  }
}

void Server::use(Logger &logger) noexcept { logger_ = &logger; }

void Server::use(ThreadPool &thread_pool) noexcept {
  thread_pool_ = &thread_pool;
}

void Server::use(EventLoop &event_loop) noexcept { event_loop_ = &event_loop; }

void Server::use(Router &router) noexcept { router_ = &router; }

void Server::run() {
  if (logger_ == nullptr || router_ == nullptr || event_loop_ == nullptr ||
      thread_pool_ == nullptr) {
    throw std::logic_error("Server dependencies are not configured");
  }

  std::cout << "Server is running on port " << port_ << "...\n";

  server_channel_ = std::make_unique<Channel>(server_fd_, EPOLLIN);
  server_channel_->setReadCallback([this] { acceptClient(); });
  event_loop_->addChannel(server_channel_.get());
  event_loop_->run();
}

void Server::acceptClient() {
  sockaddr_in client_address{};
  socklen_t address_length = sizeof(client_address);
  int client_fd = accept(server_fd_,
                         reinterpret_cast<sockaddr *>(&client_address),
                         &address_length);

  if (client_fd < 0) {
    std::cerr << "Failed to accept client connection\n";
    return;
  }

  thread_pool_->enqueue([this, client_fd] { handleClient(client_fd); });
}

void Server::handleClient(int client_fd) {
  Request request;
  RequestState state = RequestState::RequestLine;
  std::string request_buffer;
  bool parsed = false;

  while (!parsed) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
      break;
    }
    request_buffer.append(buffer, bytes_read);
    parsed = request.parse(request_buffer, state);
  }

  if (!parsed) {
    close(client_fd);
    return;
  }

  Response response = router_->handle(request.path(), request);
  std::string serialized_data = response.serialize();
  ssize_t sent = send(client_fd, serialized_data.c_str(), serialized_data.size(),
                      MSG_NOSIGNAL);
  if (sent < 0 && errno != EINTR) {
    std::cerr << "Failed to send response\n";
  }

  close(client_fd);
}