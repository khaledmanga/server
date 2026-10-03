#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
 private:
  std::vector<std::thread> workers;
  std::deque<std::function<void()>> tasks;
  std::condition_variable cv;
  std::mutex lock;
  bool shutting_down = false;

 public:
  ThreadPool(size_t numThreads);
  ~ThreadPool();
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  void enqueue(std::function<void()> task);
  void worker();
  void shutdown();
};