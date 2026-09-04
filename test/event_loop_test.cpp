#include <gtest/gtest.h>

#include <sys/epoll.h>
#include <unistd.h>

#include "../src/channel.hpp"
#include "../src/event_loop.hpp"

TEST(EventLoopTest, DispatchesReadableChannelAndStops) {
  int pipe_fds[2];
  ASSERT_EQ(pipe(pipe_fds), 0);

  EventLoop event_loop;
  Channel channel(pipe_fds[0], EPOLLIN);
  channel.setReadCallback([&event_loop, &pipe_fds] {
    char value;
    EXPECT_EQ(read(pipe_fds[0], &value, 1), 1);
    event_loop.stop();
  });

  event_loop.addChannel(&channel);
  ASSERT_EQ(write(pipe_fds[1], "x", 1), 1);
  event_loop.run();

  close(pipe_fds[0]);
  close(pipe_fds[1]);
}