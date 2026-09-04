#pragma once

#include <memory>

#include "channel.hpp"
#include "event_loop.hpp"
#include "logger.hpp"
#include "router.hpp"
#include "thread_pool.hpp"

class Server {
public:
  explicit Server(int port);
  ~Server();

  void use(Logger &logger) noexcept;
  void use(ThreadPool &thread_pool) noexcept;
  void use(EventLoop &event_loop) noexcept;
  void use(Router &router) noexcept;
  void run();

private:
  static constexpr int BUFFER_SIZE = 4096;

  int port_;
  int server_fd_ = -1;
  Logger *logger_ = nullptr;
  Router *router_ = nullptr;
  EventLoop *event_loop_ = nullptr;
  ThreadPool *thread_pool_ = nullptr;
  std::unique_ptr<Channel> server_channel_;

  void setupSocket();
  void acceptClient();
  void handleClient(int client_fd);
};