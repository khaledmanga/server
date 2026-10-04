#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <unordered_set>

#include "../context/connection.h"
#include "../event_loop/event_loop.h"
#include "../http_parser/http_parser.h"
#include "../logger/logger.h"
#include "../router/router.h"

class ThreadPool;

class Server {
 public:
  Server(int port_, const Router &router_, Logger &logger_, ThreadPool &thread_pool_);
  void run();

 private:
  void createSocket();
  void bindSocket();
  void listenSocket();
  void handleClient(int fd);
  void acceptClient();
  void signalHandler(int signal);
  void gracefulShutdown();
  void setupSocket();
  void setupSignals();

  EventLoop event_loop;
  Logger &logger;
  ThreadPool &thread_pool;
  int port;
  int server_fd;
  int signal_fd;
  const Router &router;
  std::unordered_set<int> pending_clients;
};
