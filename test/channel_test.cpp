#include <gtest/gtest.h>

#include <sys/epoll.h>

#include "../src/channel.hpp"

TEST(ChannelTest, StoresFileDescriptorAndEvents) {
  Channel channel(42, EPOLLIN | EPOLLOUT);

  EXPECT_EQ(channel.fd(), 42);
  EXPECT_EQ(channel.event(), EPOLLIN | EPOLLOUT);
}

TEST(ChannelTest, HandlesReadCallback) {
  Channel channel(1, EPOLLIN);
  bool called = false;
  channel.setReadCallback([&called] { called = true; });

  channel.handleEvent(EPOLLIN);

  EXPECT_TRUE(called);
}

TEST(ChannelTest, HandlesWriteCallback) {
  Channel channel(1, EPOLLOUT);
  bool called = false;
  channel.setWriteCallback([&called] { called = true; });

  channel.handleEvent(EPOLLOUT);

  EXPECT_TRUE(called);
}