#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

enum class LogLevel : int { DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 };

const std::unordered_map<LogLevel, std::string> LogLevelMap = {
  {LogLevel::DEBUG, "DEBUG"},
  {LogLevel::INFO, "INFO"},
  {LogLevel::WARN, "WARN"},
  {LogLevel::ERROR, "ERROR"}
};

class LogRecord {
private:
  std::string timestamp_;
  LogLevel level_ = LogLevel::INFO;
  std::thread::id thread_id_;
  std::string message_;

public:
  std::string msg() const;
  std::string timestamp() const;
  LogLevel level() const;
  std::thread::id thread_id() const;
  std::string getMsg() const;
  void set(const LogLevel &log_level, std::thread::id thread_id,
           const std::string &message);
};

class Sink {
public:
  virtual ~Sink() = default;
  virtual void write(const LogRecord &log_record) = 0;
};

class Terminal : public Sink {
public:
  void write(const LogRecord &log_record) override;
};

class Logger {
private:
  std::vector<std::unique_ptr<Sink>> sinks;
  std::mutex mutex_;

public:
  void addSink(std::unique_ptr<Sink> sink);
  void log(LogLevel level, const std::string &message);
};
