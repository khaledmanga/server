#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
private:
  std::vector<std::thread> workers;
  std::vector<std::function<void()>> tasks;
  std::condition_variable cv;
  std::mutex lock;
  bool shutting_down = false;
  size_t numThreads;

public:
  ThreadPool(size_t numThreads);
  void enqueue(std::function<void()> task);
  void worker();
  void shutdown();
}