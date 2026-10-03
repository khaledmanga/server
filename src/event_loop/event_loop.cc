#include "event_loop.h"

EventLoop::EventLoop() {
  this->epoll_fd = epoll_create1(0);
  this->running = true;
}

EventLoop::~EventLoop() { this->stop(); }

void EventLoop::run(int server_fd, std::function<void()> accept_handler,
                    std::function<void(int)> client_handler) {
  add_event(server_fd);

  while (running) {
    epoll_event events[MAX_EVENTS];

    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for (int i = 0; i < n; i++) {
      int fd = events[i].data.fd;

      if (fd == server_fd) {
        accept_handler();
      } else {
        client_handler(fd);
      }
    }
  }
}

void EventLoop::add_event(int fd) {
  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &event);
}

void EventLoop::remove_event(int fd) {
  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, &event);
}

void EventLoop::update_event(int fd) {
  epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, fd, &event);
}

void EventLoop::stop() {
  this->running = false;
  close(this->epoll_fd);
}