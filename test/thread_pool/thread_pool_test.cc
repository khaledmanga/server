#include "../../src/thread_pool/thread_pool.h"

#include <gtest/gtest.h>

#include <atomic>
#include <stdexcept>

TEST(ThreadPoolTest, ShutdownDrainsQueuedTasks) {
  ThreadPool thread_pool(2);
  std::atomic<int> completed_tasks{0};

  for (int i = 0; i < 10; ++i) {
    thread_pool.enqueue([&completed_tasks] { ++completed_tasks; });
  }

  thread_pool.shutdown();

  EXPECT_EQ(completed_tasks.load(), 10);
}

TEST(ThreadPoolTest, RejectsTasksAfterShutdown) {
  ThreadPool thread_pool(1);
  thread_pool.shutdown();

  EXPECT_THROW(thread_pool.enqueue([] {}), std::runtime_error);
}
