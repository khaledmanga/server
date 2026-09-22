#pragma once

#include <condition_variable>
#include <cstddef>

#include <mutex>
#include <optional>
#include <queue>
#include <utility>


template <typename T>
class BoundedQueue {
 public:
  explicit BoundedQueue(std::size_t capacity);

  bool push(T value);
  std::optional<T> pop();
  void shutdown();

 private:
  std::queue<T> queue_;
  std::size_t capacity_;

  std::mutex mutex_;
  std::condition_variable cv_;
  bool shutdown_ = false;
};

template <typename T>
BoundedQueue<T>::BoundedQueue(std::size_t capacity)
    : capacity_(capacity) {}

template <typename T>
bool BoundedQueue<T>::push(T value) {
  {
    std::lock_guard lock(mutex_);

    if (queue_.size() >= capacity_ || shutdown_) {
      return false;
    }

    queue_.push(std::move(value));
  }

  cv_.notify_one();


  return true;
}

template <typename T>
std::optional<T> BoundedQueue<T>::pop() {
  std::unique_lock lock(mutex_);

  cv_.wait(lock, [this] {
    return !queue_.empty() || shutdown_;
  });

  if (queue_.empty()) {
    return std::nullopt;
  }

  T value = std::move(queue_.front());
  queue_.pop();

  return value;
}

template <typename T>
void BoundedQueue<T>::shutdown() {
  {
    std::lock_guard lock(mutex_);
    shutdown_ = true;
  }

  cv_.notify_all();
}
