#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "request.hpp"
#include "response.hpp"

#include <iostream>

constexpr int PORT = 8004;
constexpr int BUFFER_SIZE = 4096;

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    std::cerr << "Failed to create socket\n";
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, reinterpret_cast<sockaddr *>(&server_addr),
           sizeof(server_addr)) < 0) {

    std::cerr << "Failed to bind\n";
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, SOMAXCONN) < 0) {
    std::cerr << "Failed to listen\n";
    close(server_fd);
    return 1;
  }

  std::cout << "Server is running on port " << PORT << "...\n";

  while (true) {
    sockaddr_in client_address{};
    socklen_t addr_len = sizeof(client_address);

    int client_fd = accept(
        server_fd, reinterpret_cast<sockaddr *>(&client_address), &addr_len);

    if (client_fd < 0) {
      std::cerr << "Failed to accept client connection\n";
      continue;
    }

    Request request;
    RequestState state = RequestState::RequestLine;
    std::string req_buffer;

    bool parsed = false;

    while (!parsed) {
      char buffer[BUFFER_SIZE];

      ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));

      if (bytes_read <= 0) {
        break;
      }

      req_buffer.append(buffer, bytes_read);

      parsed = request.parse(req_buffer, state);
    }

    if (parsed) {
      Response response;
      std::string serialized_data = response.serialize();

      ssize_t sent = send(client_fd,serialized_data.c_str(), serialized_data.size(), MSG_NOSIGNAL);
    
      if (sent < 0) {
        if (errno == EINTR) continue;
        break; 
      }
    }

    close(client_fd);
  }

  close(server_fd);

  return 0;
}
