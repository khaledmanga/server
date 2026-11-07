#include "include/error/error.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Server {
public:
  int port;
  int server_fd;
  std::unordered_map<int, std::unique_ptr<Connection>> connections;

  Server(int port);
  ~Server();

  void create_socket();
  void bind_socket();
  void listen_socket();
  void accept_connection();
  void handle_client(int client_socket);
  void run();
};