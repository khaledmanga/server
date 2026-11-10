#include "server.hpp"

void set_nonblocking(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void add_to_epoll(int epfd, int fd, uint32_t events) {
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = events;

  epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void update_to_epoll(int epfd, int fd, uint32_t events) {
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = events;

  epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void delete_from_epoll(int epfd, int fd) {
  epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
}


Server::Server(int port) : port(port), server_fd(-1) {
  this->epoll_fd = epoll_create1(EPOLL_CLOEXEC);
  this->running_event_loop = false;
}

Server::~Server() {
  if (server_fd != -1) {
    close(server_fd);
  }

  if(this->epoll_fd != -1) {
    close(this->epoll_fd);
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
  set_nonblocking(server_fd);
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

  add_to_epoll(this->epoll_fd, this->server_fd, EPOLLIN);
}

void Server::handle_connections() {
  this->running_event_loop = true;

  while(this->running_event_loop) {
    int num_ready = epoll_wait(this->epoll_fd, this->events.data(), 2048, -1);
    if (num_ready < 0) {
      if (errno == EINTR) {
        continue;
      }
      server_error::perror(server_error::Error{server_error::ErrorCode::READ_FAILED, "Failed to wait for socket events", errno});
      continue;
    }

    for(int i = 0; i < num_ready; ++i) {
      const int fd = this->events[i].data.fd;
      const uint32_t ready_events = this->events[i].events;

      if(fd == this->server_fd) {
        this->accept_connection();
        continue;
      }

      auto connection = this->connections.find(fd);
      if (connection == this->connections.end()) {
        continue;
      }

      Connection &client = *connection->second;
      if (ready_events & EPOLLIN) {
        client.handle_read();
      }
      if (client.socket_fd != -1 && (ready_events & EPOLLOUT)) {
        client.handle_write();
      }
      if (client.socket_fd != -1 && (ready_events & (EPOLLERR | EPOLLHUP))) {
        close(client.socket_fd);
        client.socket_fd = -1;
      }

      if (client.socket_fd == -1) {
        delete_from_epoll(this->epoll_fd, fd);
        this->connections.erase(connection);
        continue;
      }

      if (!client.write_buffer.empty()) {
        update_to_epoll(this->epoll_fd, fd, EPOLLIN | EPOLLOUT);
      } else {
        update_to_epoll(this->epoll_fd, fd, EPOLLIN);
      }
    }
  }
}

void Server::accept_connection() {
  while (true) {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int client_socket = accept(this->server_fd, (struct sockaddr *)&address, &addrlen);

    if (client_socket < 0) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return;
      }
      server_error::perror(server_error::Error{server_error::ErrorCode::ACCEPT_FAILED, "Failed to accept connection", errno});
      return;
    }

    set_nonblocking(client_socket);
    add_to_epoll(this->epoll_fd, client_socket, EPOLLIN);
    this->connections[client_socket] = std::make_unique<Connection>(client_socket, router, logger);
  }
}

void Server::run() {
  create_socket();
  bind_socket();
  listen_socket();

  std::cout << "Server is running on port " << this->port << std::endl;

  handle_connections();
}

void Server::use(const Router &router) {
  this->router.routes.clear();
  for (const auto &route : router.routes) {
    this->router.routes[route.first] = std::make_unique<Route>(*route.second);
  }
}
