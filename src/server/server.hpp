#include "include/error/error.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Server {
public:
  int port;
  int server_fd;

  Server(int port);
  ~Server();

  void create_socket();
  void bind_socket();
  void listen_socket();
  void accept_connection();
  void handle_client(int client_socket);
  void run();
};