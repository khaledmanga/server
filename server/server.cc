#include "server.h"
#include <unistd.h>

Server::Server() {
  this->running = true;
  this->port = 8080;
  this->server_fd = -1;
}

Server::~Server() {
  if (this->server_fd >= 0) {
    close(this->server_fd);
  }
}

void Server::set_port(int port) { this->port = port; }

void Server::setup_socket() {
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(this->port);

  this->bind_socket(addr);
}

void Server::bind_socket(struct sockaddr_in &addr) { bind(this->server_fd, (struct sockaddr *)&addr, sizeof(addr)); }

void Server::listen_sockets() { listen(this->server_fd, SOMAXCONN); }

void Server::accept_client() {
  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);

  int fd_client = accept(this->server_fd, (struct sockaddr *)&client, &client_len);
  if (fd_client < 0) {
    return;
  }

  Connection connection(fd_client);
  connection.run();
}

void Server::run() {
  this->setup_socket();
  this->listen_sockets();

  std::cout << "Server is running in port: " << this->port << std::endl;

  while (this->running) {
    this->accept_client();
  }
}