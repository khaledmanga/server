#include "channel.hpp"

Channel::Channel(int fd, uint32_t event) {
  this->fd_ = fd;
  this->event_ = event;
}

int Channel::fd() const { return this->fd_; }

uint32_t Channel::event() const { return this->event_; }

void Channel::setReadCallback(Callback callback) {
  this->read_callback_ = callback;
}

void Channel::setWriteCallback(Callback callback) {
  this->write_callback_ = callback;
}

void Channel::handleEvent(uint32_t event) {
  if (event & EPOLLIN) {
    if (this->read_callback_) {
      read_callback_();
    }
  } else if (event & EPOLLOUT) {
    if (this->write_callback_) {
      write_callback_();
    }
  }
}
