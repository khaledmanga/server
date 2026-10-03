#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t numThreads) : numThreads(numThreads) {
  for (size_t i = 0; i < numThreads; ++i) {
    std::thread t([this] { this->worker(); });
    this->workers.push_back(std::move(t));
  }
}

ThreadPool::~ThreadPool() { shutdown(); }

void ThreadPool::enqueue(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(this->lock);
    this->tasks.push_back(task);
  }

  this->cv.notify_one();
}

void ThreadPool::worker() {
  while (!this->shutting_down) {
    std::function<void()> task;

    {
      std::lock_guard<std::mutex> lock(this->lock);
      this->cv.wait(
          lock, [this] { return !this->tasks.empty() || this->shutting_down; })

          task = std::move(this->tasks.back());
      this->tasks.pop_back();
    }

    task();
  }
}

void ThreadPool::shutdown() {
  {
    std::lock_guard<std::mutex> lock(this->lock);
    this->shutting_down = true;
  }

  this->cv.notify_all();
}