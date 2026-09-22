#include "logger.hpp"

#include <iostream>

#include <sstream>


#include "common.hpp"

void LogRecord::set(LogLevel level, std::thread::id thread_id,
                    const std::string& message) {
  level_ = level;
  thread_id_ = thread_id;
  message_ = message;
  timestamp_ = getCurrentTime();
}

std::string LogRecord::msg() const {
  return message_;
}

std::string LogRecord::timestamp() const {

  return timestamp_;

}


LogLevel LogRecord::level() const {
  return level_;
}

std::thread::id LogRecord::thread_id() const {

  return thread_id_;
}

std::string LogRecord::getMsg() const {
  std::ostringstream output;

  output << timestamp_ << " [" << LogLevelMap.at(level_) << "] "
         << thread_id_ << " '" << message_ << "'";

  return output.str();
}

void Terminal::write(const LogRecord& log_record) {
  std::cout << log_record.getMsg() << std::endl;

}


Logger::Logger()
    : worker_(&Logger::workLoop, this) {}

Logger::~Logger() {
  {

    std::lock_guard lock(mutex_);
    running_ = false;
  }

  cv_.notify_one();

  if (worker_.joinable()) {
    worker_.join();
  }
}

void Logger::workLoop() {
  while (true) {
    std::unique_lock lock(mutex_);

    cv_.wait(lock, [this] {
      return !queue_.empty() || !running_;
    });

    if (!running_ && queue_.empty()) {
      break;
    }


    LogRecord record = std::move(queue_.front());
    queue_.pop();

    lock.unlock();

    for (const auto& sink : sinks_) {
      sink->write(record);
    }

  }
}

void Logger::addSink(std::unique_ptr<Sink> sink) {
  std::lock_guard lock(mutex_);
  sinks_.push_back(std::move(sink));

}


void Logger::log(LogLevel level, const std::string& message) {
  LogRecord record;
  record.set(level, std::this_thread::get_id(), message);

  {
    std::lock_guard lock(mutex_);
    queue_.push(std::move(record));
  }

  cv_.notify_one();
}
