#pragma once

#include <sys/epoll.h>
#include <unordered_map>
#include "channel.hpp"
#include <stdexcept>
#include <unistd.h>

class EventLoop {
private:
  bool running_;
  int epoll_fd_;
  epoll_event event_[64];
  std::unordered_map<int, Channel*> channels_;

public:
  EventLoop();
  ~EventLoop();
  void run();
  void stop();
  void addChannel(Channel *channel);
  void updateChannel(Channel *channel);
  void removeChannel(Channel *channel);
};
