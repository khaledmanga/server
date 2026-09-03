#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <string>

#include "event_loop.hpp"
#include "request.hpp"
#include "response.hpp"
#include "router.hpp"
#include "thread_pool.hpp"

constexpr int PORT = 8004;
constexpr int BUFFER_SIZE = 4096;

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create socket\n";
    return 1;
  }

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

  Router router;
  router.add("/", [](Request &request) {
    Response response;
    response.setBody("<h1>Hello World</h1>");
    return response;
  });

  EventLoop eventloop;
  ThreadPool thread_pool(std::thread::hardware_concurrency());

  std::function<void(int)> handle_client;
  handle_client = [&](int client_fd) {
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

    if (!parsed) {
      close(client_fd);
      return;
    }

    const auto *matched_pair = router.match(request.path());
    if (matched_pair == nullptr) {
      Response response;
      StatusLine status_line;
      status_line.setStatusCode(404).setReasonPhrase("Not Found");
      response.setStatusLine(status_line).setBody("<h1>404 Not Found</h1>");
      std::string serialized_data = response.serialize();
      send(client_fd, serialized_data.c_str(), serialized_data.size(), MSG_NOSIGNAL);
    } else {
      Response response = matched_pair->second(request);
      std::string serialized_data = response.serialize();
      ssize_t sent = send(client_fd, serialized_data.c_str(), serialized_data.size(),
                          MSG_NOSIGNAL);
      if (sent < 0 && errno != EINTR) {
        std::cerr << "Failed to send response\n";
      }
    }

    close(client_fd);
  };

  Channel server_channel(server_fd, EPOLLIN);
  server_channel.setReadCallback([&] {
    sockaddr_in client_address{};
    socklen_t addr_len = sizeof(client_address);
    int client_fd = accept(
        server_fd, reinterpret_cast<sockaddr *>(&client_address), &addr_len);

    if (client_fd < 0) {
      std::cerr << "Failed to accept client connection\n";
      return;
    }

    thread_pool.enqueue([&, client_fd] { handle_client(client_fd); });
  });

  eventloop.addChannel(&server_channel);
  eventloop.run();

  close(server_fd);
  return 0;
}
