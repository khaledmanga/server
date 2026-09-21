#pragma once

#include <atomic>
#include <stdexcept>
#include <unordered_map>
#include <cerrno>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>

#include "channel.hpp"

class EventLoop {
private:
  bool running_;
  int epoll_fd_;

  int event_fd_;


  epoll_event event_[64];

  std::atomic<bool>& shutdown_requested_;

  std::unordered_map<int, Channel*> channels_;

public:
  explicit EventLoop(std::atomic<bool>& shutdown_requested);
  ~EventLoop();

  void run();
  void stop();

  void addChannel(Channel* channel);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  int eventFd() const;
};
