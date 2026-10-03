#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../context/connection.hpp"
#include "../event_loop/event_loop.hpp"
#include "../http_parser/http_parser.hpp"
#include "../router/router.hpp"
#include "../thread_pool/thread_pool.hpp"

class Server {
public:
  Server(int port_, const Router &router_);
  void run();
  void use(ThreadPool &thread_pool);

private:
  void createSocket();
  void bindSocket();
  void listenSocket();
  void handleClient(int fd);
  void acceptClient();

  EventLoop event_loop;
  ThreadPool thread_pool;
  int port;
  int server_fd;
  const Router &router;
};
