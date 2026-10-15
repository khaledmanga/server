#pragma once

#include "connection.h"
#include "logger/logger.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class Server {
private:
  int port;
  int server_fd;
  bool running;
  Logger logger;
  void setup_socket();
  void bind_socket(struct sockaddr_in &);
  void listen_sockets();
  void accept_client();

public:
  Server();
  void set_port(int port);
  void use(Logger logger);
  void run();
  ~Server();
};