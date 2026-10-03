#include "server.hpp"

Server::Server(int port_, const Router &router_)
    : port(port_), server_fd(-1), router(router_) {}

void Server::createSocket() {
  int server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;

  if (server_fd_ == -1) {
    perror("Socket create");
    return;
  }

  this->server_fd = server_fd_;

  setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Server::bindSocket() {
  sockaddr_in addr{};

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(this->port);

  if (bind(this->server_fd, (sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Bind socket");
    return;
  }
}

void Server::listenSocket() {
  if (listen(this->server_fd, SOMAXCONN) == -1) {
    perror("Listen socket");
    close(this->server_fd);
    return;
  }
}

void Server::acceptClient() {
  sockaddr_in client_addr{};
  socklen_t client_len = sizeof(client_addr);

  int client_fd =
      accept(this->server_fd, (sockaddr *)&client_addr, &client_len);

  if (client_fd == -1) {
    perror("Accept socket");
    return;
  }

  this->event_loop.add_event(client_fd);
}

void Server::handleClient(int fd) {
  Connection connection(fd, this->router);

  connection.handle_read();

  this->event_loop.remove_event(fd);
  close(fd);
}

void Server::run() {
  createSocket();
  bindSocket();
  listenSocket();

  std::cout << "Server is listening on port " << this->port << std::endl;

  this->event_loop.run(
      this->server_fd, 
      [this]() { this->acceptClient(); },
      [this](int fd) { this->handleClient(fd); });
}