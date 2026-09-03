#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  using Task = std::function<void()>;

private:
  std::vector<std::thread> workers_;
  std::queue<Task> tasks_;
  std::mutex mtx_;
  std::condition_variable cv_;
  bool stop_ = false;

public:
  ThreadPool(int n);
  void enqueue(Task task);
  void worker();
  ~ThreadPool();
};
