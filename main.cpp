#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8000);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Failed binding" << std::endl;
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, SOMAXCONN) < 0) {
    std::cerr << "Failed listening" << std::endl;
    close(server_fd);
    return 1;
  }

  std::cout << "Server is running on port 8000..." << std::endl;

  sockaddr_in client_address{};
  socklen_t addr_len = sizeof(client_address);

  while (true) {
    int client_fd =
        accept(server_fd, (struct sockaddr*)&client_address, &addr_len);
    if (client_fd < 0) {
      std::cerr << "Failed to accept client connection" << std::endl;
      continue;
    }

    char buffer[4096] = {0};
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));

    if (bytes_read > 0) {
      send(client_fd, buffer, bytes_read, 0);
    }

    close(client_fd);
  }

  close(server_fd);
  return 0;
}

