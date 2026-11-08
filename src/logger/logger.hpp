#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include "include/common.h"
#include <memory>
#include <string>
#include <vector>

struct LogRecord {
  LOG_LEVEL level;
  std::string message;
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
  std::vector<std::unique_ptr<Sink>> sinks;

  void add_sink(std::unique_ptr<Sink> sink);
  void log(LOG_LEVEL level, const std::string &message) const;
  void INFO(const std::string &message) const;
  void DEBUG(const std::string &message) const;
  void ERROR(const std::string &message) const;
  void WARN(const std::string &message) const;
};

#endif