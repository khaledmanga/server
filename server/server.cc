#include "server.h"
#include <cerrno>
#include <system_error>
#include <unistd.h>
#include <utility>

Server::Server(Router router) : port(8080), server_fd(-1), epoll_fd(-1), running(true), router(std::move(router)) {
  this->init_epoll();
}

Server::~Server() {
  if (this->server_fd >= 0) {
    close(this->server_fd);
  }
  if (this->epoll_fd >= 0) {
    close(this->epoll_fd);
  }
}

void Server::init_epoll() { this->epoll_fd = epoll_create1(0); }

void Server::add_event(int fd, FdCtx *fdctx) {
  struct epoll_event ev{};
  ev.data.ptr = fdctx;
  ev.events = EPOLLIN;

  epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

void Server::update_event(int fd, FdCtx *fdctx) {
  struct epoll_event ev{};
  ev.data.ptr = fdctx;
  ev.events = EPOLLOUT;

  epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

void Server::remove_event(int fd) { epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, nullptr); }

void Server::set_port(int port) { this->port = port; }

void Server::setup_socket() {
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int reuse_address = 1;
  setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address));

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(this->port);

  this->bind_socket(addr);
  this->listen_sockets();

  auto *fdctx = new FdCtx{};
  fdctx->fd = this->server_fd;
  this->add_event(this->server_fd, fdctx);
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

  int flags = fcntl(fd_client, F_GETFL, 0);
  fcntl(fd_client, F_SETFL, flags | O_NONBLOCK);

  Connection *conn = new Connection(fd_client, this->router);

  auto *fdctx = new FdCtx{};
  fdctx->fd = fd_client;
  fdctx->connection_ptr = conn;

  this->add_event(fd_client, fdctx);
}

void Server::close_connection(FdCtx *ctx) {
  this->remove_event(ctx->fd);
  close(ctx->fd);
  delete ctx->connection_ptr;
  delete ctx;
}

void Server::handle_connection_event(FdCtx *ctx, uint32_t events) {
  Connection *conn = ctx->connection_ptr;

  if (events & (EPOLLERR | EPOLLHUP)) {
    this->close_connection(ctx);
    return;
  }

  if (events & EPOLLIN) {
    conn->handle_read();
    if (conn->parser_status == HttpStatus::End) {
      conn->handle_router();
      this->update_event(ctx->fd, ctx);
    } else if (conn->parser_status == HttpStatus::Error) {
      this->close_connection(ctx);
      return;
    }
  }

  if (events & EPOLLOUT) {
    conn->handle_write();
    if (conn->write_buffer.empty()) {
      this->close_connection(ctx);
    }
  }
}

void Server::run() {
  this->setup_socket();
  logger.INFO(std::this_thread::get_id(), "Server is running on port: " + std::to_string(this->port));

  while (this->running) {
    int number_ready_events = epoll_wait(this->epoll_fd, this->events, MAX_EVENTS, -1);

    for (int i = 0; i < number_ready_events; ++i) {
      auto *ctx = static_cast<FdCtx *>(this->events[i].data.ptr);

      if (ctx->fd == this->server_fd) {
        this->accept_client();
        continue;
      }

      this->handle_connection_event(ctx, this->events[i].events);
    }
  }
}

void Server::use(Logger logger) { this->logger = std::move(logger); }