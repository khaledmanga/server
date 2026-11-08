#ifndef SERVER_HPP
#define SERVER_HPP

#include "connection/connection.hpp"
#include "include/error/error.hpp"
#include "router/router.hpp"
#include <arpa/inet.h>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class Server {
public:
  int port;
  int server_fd;
  std::unordered_map<int, std::unique_ptr<Connection>> connections;
  Router router;

  Server(int port);
  ~Server();

  void create_socket();
  void bind_socket();
  void listen_socket();
  void accept_connection();
  void handle_client(int client_socket);
  void run();
  void use(const Router& router);
};

#endif