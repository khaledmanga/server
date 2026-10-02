#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "../http_parser/http_parser.hpp"
#include "../context/connection.hpp"

class Server {
public:
  Server(int port_);
  void run();

private:
  void createSocket();
  void bindSocket();
  void listenSocket();
  void acceptClient();

  int port;
  int server_fd;
};
