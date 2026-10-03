#pragma once

#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../constant/common.hpp"

struct LogRecord {
  Log::Level level;
  std::string message;
  std::thread::id threadId;
  std::string timestamp;
};

class Sink {
 public:
  virtual ~Sink() = default;
  virtual void log(const LogRecord &record) = 0;
};

class Terminal : public Sink {
 public:
  void log(const LogRecord &record) override;
};

class Logger {
 public:
  Logger();
  ~Logger();
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void Info(const std::string &message);
  void Debug(const std::string &message);
  void Error(const std::string &message);
  void Warn(const std::string &message);
  void log(Log::Level level, const std::string &message);

 private:
  std::vector<Sink *> sinks;
  std::mutex mutex;
};