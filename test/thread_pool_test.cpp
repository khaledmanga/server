#include <gtest/gtest.h>

#include <chrono>
#include <future>

#include "../src/context.hpp"
#include "../src/thread_pool.hpp"

TEST(ThreadPoolTest, ExecutesEnqueuedTask) {
  AppContext::shutting_down.store(false);

  ThreadPool thread_pool(1);

  std::promise<int> result;
  std::future<int> future = result.get_future();

  thread_pool.enqueue([&result] {
    result.set_value(42);
  });

  EXPECT_EQ(
      future.wait_for(std::chrono::seconds(1)),
      std::future_status::ready);


  EXPECT_EQ(future.get(), 42);
}
