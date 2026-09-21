#include "event_loop.hpp"

EventLoop::EventLoop(std::atomic<bool> &shutdown_requested)

    : running_(false), epoll_fd_(-1), event_fd_(-1),
      shutdown_requested_(shutdown_requested) {
  this->event_fd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

  if (this->event_fd_ == -1) {
    throw std::runtime_error("eventfd failed");
  }

  this->epoll_fd_ = epoll_create1(0);

  if (this->epoll_fd_ == -1) {
    close(this->event_fd_);
    throw std::runtime_error("epoll_create1 failed");
  }

  epoll_event event{};

  event.events = EPOLLIN;
  event.data.fd = this->event_fd_;

  if (epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, this->event_fd_, &event) ==
      -1) {
    close(this->event_fd_);
    close(this->epoll_fd_);

    throw std::runtime_error("Add eventfd to epoll failed");
  }
}

EventLoop::~EventLoop() {
  if (this->event_fd_ != -1) {
    close(this->event_fd_);
  }

  if (this->epoll_fd_ != -1) {
    close(this->epoll_fd_);
  }
}

void EventLoop::run() {
  this->running_ = true;

  while (this->running_ && !this->shutdown_requested_.load()) {

    int count = epoll_wait(this->epoll_fd_, this->event_, 64, -1);

    if (count == -1) {
      if (errno == EINTR) {
        continue;
      }

      throw std::runtime_error("epoll_wait failed");
    }

    for (int i = 0; i < count; ++i) {
      epoll_event event = this->event_[i];

      int fd = event.data.fd;
      uint32_t flag = event.events;

      if (fd == this->event_fd_) {
        uint64_t value;

        ssize_t n = read(this->event_fd_,

                         &value, sizeof(value));

        std::cout << "Grateful shutdown\n";

        if (n == sizeof(value)) {
          this->shutdown_requested_.store(true);
        }

        continue;
      }

      auto it = this->channels_.find(fd);

      if (it == this->channels_.end()) {
        continue;
      }

      Channel *channel = it->second;

      channel->handleEvent(flag);
    }
  }

  this->running_ = false;
}

void EventLoop::stop() {

  this->running_ = false;
  this->shutdown_requested_.store(true);
}

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
  if (epoll_ctl(this->epoll_fd_,

                EPOLL_CTL_DEL,

                channel->fd(), nullptr) == -1) {
    throw std::runtime_error("Remove channel failed");
  }

  this->channels_.erase(channel->fd());
}

int EventLoop::eventFd() const { return this->event_fd_; }
