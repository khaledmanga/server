#ifndef SERVER_HPP
#define SERVER_HPP

#include "connection/connection.hpp"
#include "include/error/error.hpp"
#include "router/router.hpp"
#include "logger/logger.hpp"
#include <arpa/inet.h>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

class Server {
public:
  int port;
  int server_fd;
  int running_event_loop;
  int epoll_fd;
  std::vector<struct epoll_event> events = std::vector<struct epoll_event>(2048);
  std::unordered_map<int, std::unique_ptr<Connection>> connections;
  Router router;
  Logger logger;

  Server(int port);
  ~Server();

  void create_socket();
  void bind_socket();
  void listen_socket();
  void handle_connections();
  void accept_connection();
  void run();
  void use(const Router &router);
  void use(const Logger &logger);
};

#endif