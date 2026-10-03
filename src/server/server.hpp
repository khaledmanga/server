#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "../context/connection.hpp"
#include "../event_loop/event_loop.hpp"
#include "../http_parser/http_parser.hpp"
#include "../logger/logger.hpp"
#include "../router/router.hpp"

class Server {
 public:
  Server(int port_, const Router &router_, Logger &logger_);
  void run();

 private:
  void createSocket();
  void bindSocket();
  void listenSocket();
  void handleClient(int fd);
  void acceptClient();

  EventLoop event_loop;
  Logger &logger;
  int port;
  int server_fd;
  const Router &router;
};
