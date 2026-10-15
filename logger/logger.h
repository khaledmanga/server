#pragma once

#include <string>
#include <thread>
#include <vector>

#include "include/common.h"

class LogRecord {
public:
  std::thread::id thread_id;
  std::string timestamp;
  std::string message;
  LogLevel level;

  LogRecord() = default;
  LogRecord(std::thread::id, const std::string &, LogLevel &);
};

class Sink {
public:
  virtual void log(LogRecord &) = 0;
};

class Terminal : public Sink {
public:
  void log(LogRecord &) override;
};

class Logger {
private:
  std::vector<std::unique_ptr<Sink>> sinks;

public:
  void add_sink(std::unique_ptr<Sink> sink);
  void log(std::thread::id, const std::string &, LogLevel level);
  void INFO(std::thread::id, const std::string &);
  void DEBUG(std::thread::id, const std::string &);
  void WARN(std::thread::id, const std::string &);
  void ERROR(std::thread::id, const std::string &);
};