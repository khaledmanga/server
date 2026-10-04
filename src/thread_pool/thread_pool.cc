#include "thread_pool.h"

#include <stdexcept>
#include <utility>

ThreadPool::ThreadPool(size_t numThreads) {
  if (numThreads == 0) {
    throw std::invalid_argument("Thread pool must have at least one worker");
  }

  for (size_t i = 0; i < numThreads; ++i) {
    this->workers.emplace_back([this] { this->worker(); });
  }
}

ThreadPool::~ThreadPool() { this->shutdown(); }

void ThreadPool::enqueue(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(this->lock);
    if (this->shutting_down) {
      throw std::runtime_error("Cannot enqueue a task after thread pool shutdown");
    }
    this->tasks.push_back(std::move(task));
  }

  this->cv.notify_one();
}

void ThreadPool::worker() {
  while (true) {
    std::function<void()> task;

    {
      std::unique_lock<std::mutex> lock(this->lock);
      this->cv.wait(lock, [this] { return !this->tasks.empty() || this->shutting_down; });
      if (this->tasks.empty()) {
        return;
      }

      task = std::move(this->tasks.front());
      this->tasks.pop_front();
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

  for (std::thread &worker : this->workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}