#include "logger.h"

#include <iostream>

#include "../utils/common.h"

Logger::Logger() { sinks.push_back(new Terminal()); }

Logger::~Logger() {
  for (auto sink : sinks) {
    delete sink;
  }
}

void Logger::Info(const std::string &message) { log(Log::Level::INFO, message); }

void Logger::Debug(const std::string &message) { log(Log::Level::DEBUG, message); }

void Logger::Error(const std::string &message) { log(Log::Level::ERROR, message); }

void Logger::Warn(const std::string &message) { log(Log::Level::WARN, message); }

void Logger::log(Log::Level level, const std::string &message) {
  LogRecord record;
  record.level = level;
  record.message = message;
  record.threadId = std::this_thread::get_id();
  record.timestamp = getCurrentTime();

  std::lock_guard<std::mutex> lock(mutex);
  for (auto sink : sinks) {
    sink->log(record);
  }
}

void Terminal::log(const LogRecord &record) {
  std::string levelStr;
  switch (record.level) {
    case Log::Level::INFO:
      levelStr = "INFO";
      break;
    case Log::Level::DEBUG:
      levelStr = "DEBUG";
      break;
    case Log::Level::ERROR:
      levelStr = "ERROR";
      break;
    case Log::Level::WARN:
      levelStr = "WARN";
      break;
  }

  std::cout << "[" << record.timestamp << "] "
            << "[" << levelStr << "] "
            << "[" << record.threadId << "] " << record.message << std::endl;
}
