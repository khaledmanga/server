#include "thread_pool.hpp"

ThreadPool::ThreadPool(int n) {
  if (n <= 0) {
    n = 1;
  }

  for (int i = 0; i < n; ++i) {
    workers_.push_back(std::thread([this] {
      worker();
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> lock(mtx_);
    stop_ = true;
  }

  cv_.notify_all();

  for (std::thread &worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void ThreadPool::worker() {
  while (true) {
    Task task;

    {
      std::unique_lock<std::mutex> lock(mtx_);
      cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

      if (stop_ && tasks_.empty()) {
        return;
      }

      task = std::move(tasks_.front());
      tasks_.pop();
    }

    task();
  }
}

void ThreadPool::enqueue(Task task) {
  {
    std::lock_guard<std::mutex> lock(mtx_);
    if (stop_) {
      return;
    }
    tasks_.push(std::move(task));
  }

  cv_.notify_one();
}
