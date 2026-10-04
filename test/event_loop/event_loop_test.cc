#include "../../src/event_loop/event_loop.h"

#include <gtest/gtest.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <thread>

TEST(EventLoopTest, StopWakesBlockedLoop) {
  const int server_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  ASSERT_NE(server_fd, -1);

  EventLoop event_loop;
  std::atomic<bool> accept_called{false};
  std::thread loop_thread([&] {
    event_loop.run(
        server_fd,
        [&] {
          std::uint64_t value;
          read(server_fd, &value, sizeof(value));
          accept_called.store(true);
        },
        [](int) {});
  });

  const std::uint64_t value = 1;
  const ssize_t bytes_written = write(server_fd, &value, sizeof(value));
  if (bytes_written != static_cast<ssize_t>(sizeof(value))) {
    event_loop.stop();
    loop_thread.join();
    close(server_fd);
    FAIL() << "Failed to wake event loop test descriptor";
  }
  while (!accept_called.load()) {
    std::this_thread::yield();
  }

  event_loop.stop();
  loop_thread.join();
  close(server_fd);
}
