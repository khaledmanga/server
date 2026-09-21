#include <gtest/gtest.h>

#include <atomic>
#include <chrono>

#include <future>

#include "../src/thread_pool.hpp"

TEST(ThreadPoolTest, ExecutesEnqueuedTask) {
  std::atomic<bool> shutdown_requested{false};

  ThreadPool thread_pool(1, shutdown_requested);

  std::promise<int> result;
  std::future<int> future = result.get_future();

  thread_pool.enqueue([&result] { result.set_value(42); });

  EXPECT_EQ(future.wait_for(std::chrono::seconds(1)),
            std::future_status::ready);

  EXPECT_EQ(future.get(), 42);
}
