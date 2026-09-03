#include "event_loop.hpp"

EventLoop::EventLoop() {
  this->running_ = false;
  this->epoll_fd_ = epoll_create1(0);
}

EventLoop::~EventLoop() { close(this->epoll_fd_); }

void EventLoop::run() {
  this->running_ = true;

  while (running_) {
    int count = epoll_wait(this->epoll_fd_, this->event_, 64, -1);

    for (int i = 0; i < count; ++i) {
      epoll_event event = event_[i];
      int fd = event.data.fd;
      uint32_t flag = event.events;

      auto it = this->channels_.find(fd);

      if (it == this->channels_.end()) {
        continue;
      }

      Channel *channel = it->second;

      channel->handleEvent(flag);
    }
  }
}

void EventLoop::stop() { this->running_ = false; }

void EventLoop::addChannel(Channel *channel) {
  epoll_event event{};

  event.events = channel->event();
  event.data.fd = channel->fd();

  if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, channel->fd(), &event) == -1) {
    throw std::runtime_error("Add channel failed");
  }

  this->channels_[channel->fd()] = channel;
}

void EventLoop::updateChannel(Channel *channel) {
  epoll_event event{};

  event.events = channel->event();
  event.data.fd = channel->fd();

  if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_MOD, channel->fd(), &event) == -1) {
    throw std::runtime_error("Update channel failed");
  }
}

void EventLoop::removeChannel(Channel *channel) {
  if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_DEL, channel->fd(), nullptr) == -1) {
    throw std::runtime_error("Remove channel failed");
  }

  channels_.erase(channel->fd());
}
