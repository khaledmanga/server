#include <gtest/gtest.h>

#include <sys/epoll.h>
#include <unistd.h>

#include "../src/context.hpp"
#include "../src/channel.hpp"
#include "../src/event_loop.hpp"

TEST(EventLoopTest, DispatchesReadableChannelAndStops) {
  int pipe_fds[2];
  ASSERT_EQ(pipe(pipe_fds), 0);

  AppContext::shutting_down.store(false);


  EventLoop event_loop;


  Channel channel(pipe_fds[0], EPOLLIN);

  channel.setReadCallback([&pipe_fds] {
    char value;

    EXPECT_EQ(read(pipe_fds[0], &value, 1), 1);

    AppContext::shutting_down.store(true);
  });


  event_loop.addChannel(&channel);

  ASSERT_EQ(write(pipe_fds[1], "x", 1), 1);

  event_loop.run();

  close(pipe_fds[0]);

  close(pipe_fds[1]);
}
