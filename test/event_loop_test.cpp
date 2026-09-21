#include <gtest/gtest.h>
#include <atomic>

#include <sys/epoll.h>
#include <unistd.h>

#include "../src/channel.hpp"
#include "../src/event_loop.hpp"

TEST(EventLoopTest, DispatchesReadableChannelAndStops) {
  int pipe_fds[2];
  ASSERT_EQ(pipe(pipe_fds), 0);

  std::atomic<bool> shutdown_requested{false};
  EventLoop event_loop(shutdown_requested);
  
  Channel channel(pipe_fds[0], EPOLLIN);
  channel.setReadCallback([&event_loop, &pipe_fds, &shutdown_requested] {
    char value;
    EXPECT_EQ(read(pipe_fds[0], &value, 1), 1);
    shutdown_requested.store(true);
    event_loop.stop();
  });

  event_loop.addChannel(&channel);
  ASSERT_EQ(write(pipe_fds[1], "x", 1), 1);
  event_loop.run();

  close(pipe_fds[0]);
  close(pipe_fds[1]);
}

