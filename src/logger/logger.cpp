#include "logger.hpp"

void Terminal::log(const LogRecord &record) {
  std::cout << record.timestamp << " [" << to_log_level_string(record.level) << "]: " << record.message << std::endl;
}

void Logger::add_sink(std::unique_ptr<Sink> sink) { this->sinks.push_back(std::move(sink)); }

void Logger::log(LOG_LEVEL level, const std::string &message) const {
  LogRecord log_record;

  log_record.level = level;
  log_record.message = message;
  log_record.timestamp = get_timestamp();

  for (auto const &sink : this->sinks) {
    sink->log(log_record);
  }
}

void Logger::INFO(const std::string &message) const { this->log(LOG_LEVEL::INFO, message); }

void Logger::DEBUG(const std::string &message) const { this->log(LOG_LEVEL::DEBUG, message); }

void Logger::ERROR(const std::string &message) const { this->log(LOG_LEVEL::ERROR, message); }

void Logger::WARN(const std::string &message) const { this->log(LOG_LEVEL::WARN, message); }