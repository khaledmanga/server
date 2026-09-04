#include "logger.hpp"

#include <iostream>
#include <sstream>

#include "common.hpp"

void LogRecord::set(const LogLevel &log_level, std::thread::id thread_id,
                    const std::string &message) {
  level_ = log_level;
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

void Terminal::write(const LogRecord &log_record) {
  std::cout << log_record.getMsg() << std::endl;
}

void Logger::addSink(std::unique_ptr<Sink> sink) {
  std::lock_guard lock(mutex_);
  sinks.push_back(std::move(sink));
}

void Logger::log(LogLevel level, const std::string &message) {
  LogRecord record;
  record.set(level, std::this_thread::get_id(), message);

  std::lock_guard lock(mutex_);
  for (const auto &sink : sinks) {
    sink->write(record);
  }
}


