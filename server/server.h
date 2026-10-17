#pragma once

#include "connection.h"
#include "logger/logger.h"
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

const int MAX_EVENTS = 64;

struct FdCtx {
  int fd;
  Connection *connection_ptr = nullptr;
};

class Server {
private:
  int port;
  int server_fd;
  int epoll_fd;
  bool running;
  Router router;
  Logger logger;
  struct epoll_event events[MAX_EVENTS];
  void init_epoll();
  void setup_socket();
  void bind_socket(struct sockaddr_in &);
  void listen_sockets();
  void accept_client();
  void add_event(int fd, FdCtx *fdctx);
  void update_event(int fd, FdCtx *fdctx);
  void remove_event(int fd);
  void handle_connection_event(FdCtx *ctx, uint32_t events);
  void close_connection(FdCtx *ctx);

public:
  explicit Server(Router router);
  void set_port(int port);
  void use(Logger logger);
  void run();
  ~Server();
};