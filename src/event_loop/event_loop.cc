#include "event_loop.h"

#include <cerrno>
#include <cstdint>
#include <system_error>

namespace {
int check(int rc, const char *what) {
  if (rc == -1) throw std::system_error(errno, std::generic_category(), what);
  return rc;
}

int ctl(int epoll_fd, int op, int fd) {
  epoll_event event{};
  event.events = EPOLLIN;
  event.data.fd = fd;
  return epoll_ctl(epoll_fd, op, fd, &event);
}
}  // namespace

EventLoop::EventLoop() {
  epoll_fd = check(epoll_create1(EPOLL_CLOEXEC), "epoll_create1");
  wake_fd = -1;

  try {
    wake_fd = check(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC), "eventfd");
    check(ctl(epoll_fd, EPOLL_CTL_ADD, wake_fd), "epoll_ctl");
  } catch (...) {
    close(wake_fd);
    close(epoll_fd);
    throw;
  }

  running = true;
}

EventLoop::~EventLoop() {
  stop();
  close(wake_fd);
  close(epoll_fd);
}

void EventLoop::run(int server_fd, std::function<void()> accept_handler, std::function<void(int)> client_handler) {
  add_event(server_fd);

  epoll_event events[MAX_EVENTS];
  while (running) {
    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (n == -1) {
      if (errno == EINTR) continue;
      throw std::system_error(errno, std::generic_category(), "epoll_wait");
    }

    for (int i = 0; i < n && running; i++) {
      int fd = events[i].data.fd;

      if (fd == wake_fd) {
        running = false;
      } else if (fd == server_fd) {
        accept_handler();
      } else {
        client_handler(fd);
      }
    }
  }
}

void EventLoop::add_event(int fd) { ctl(epoll_fd, EPOLL_CTL_ADD, fd); }

void EventLoop::remove_event(int fd) { ctl(epoll_fd, EPOLL_CTL_DEL, fd); }

void EventLoop::update_event(int fd) { ctl(epoll_fd, EPOLL_CTL_MOD, fd); }

void EventLoop::stop() {
  running = false;
  const std::uint64_t value = 1;
  ssize_t rc = write(wake_fd, &value, sizeof(value));
  (void)rc;
}