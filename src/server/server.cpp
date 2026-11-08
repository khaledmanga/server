#include "server.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>

Server::Server(int port) : port(port), server_fd(-1) {}

Server::~Server() {
  if (server_fd != -1) {
    close(server_fd);
  }
}

void Server::create_socket() {
  int opt = 1;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    server_error::perror(server_error::Error{server_error::ErrorCode::SOCKET_CREATION_FAILED, "Failed to create socket", errno});
    exit(EXIT_FAILURE);
  }

  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Server::bind_socket() {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(this->server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    server_error::perror(server_error::Error{server_error::ErrorCode::BIND_FAILED, "Failed to bind socket", errno});
    exit(EXIT_FAILURE);
  }
}

void Server::listen_socket() {
  if (listen(this->server_fd, SOMAXCONN) < 0) {
    server_error::perror(server_error::Error{server_error::ErrorCode::LISTEN_FAILED, "Failed to listen on socket", errno});
    exit(EXIT_FAILURE);
  }
}

void Server::accept_connection() {
  int client_socket;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  client_socket = accept(this->server_fd, (struct sockaddr *)&address, &addrlen);
  if (client_socket < 0) {
    server_error::perror(server_error::Error{server_error::ErrorCode::ACCEPT_FAILED, "Failed to accept connection", errno});
    exit(EXIT_FAILURE);
  }

  handle_client(client_socket);
}

void Server::handle_client(int client_socket) {
  connections[client_socket] = std::make_unique<Connection>(client_socket, router, logger);
  connections[client_socket]->run();
}

void Server::run() {
  create_socket();
  bind_socket();
  listen_socket();

  std::cout << "Server is running on port " << this->port << std::endl;

  while (true) {
    accept_connection();
  }
}

void Server::use(const Router &router) {
  this->router.routes.clear();
  for (const auto &route : router.routes) {
    this->router.routes[route.first] = std::make_unique<Route>(*route.second);
  }
}
