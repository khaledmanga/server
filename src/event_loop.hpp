#pragma once

#include <atomic>
#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <unordered_map>

#include "context.hpp"
#include "channel.hpp"

class EventLoop {
private:
  bool running_;
  int epoll_fd_;
  int event_fd_;
  epoll_event event_[64];
  std::unordered_map<int, Channel *> channels_;

public:
  explicit EventLoop();
  ~EventLoop();

  void run();
  void stop();

  void addChannel(Channel *channel);
  void updateChannel(Channel *channel);
  void removeChannel(Channel *channel);

  int eventFd() const;
};
