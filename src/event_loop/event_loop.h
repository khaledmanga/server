#pragma once

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <atomic>
#include <functional>

const int MAX_EVENTS = 64;

class EventLoop {
 private:
  int epoll_fd;
  int wake_fd;
  std::atomic<bool> running;

 public:
  EventLoop();
  ~EventLoop();

  void run(int server_fd, std::function<void()> accept_handler, std::function<void(int)> client_handler);
  void add_event(int fd);
  void remove_event(int fd);
  void update_event(int fd);
  void stop();
};