#include "logger.h"

LogRecord::LogRecord(std::thread::id thread_id, const std::string& message, LogLevel& level){
    this->thread_id = thread_id;
    this->message = message;
    this->level = level;
    this->timestamp = getCurrentTime();
}

void Terminal::log(LogRecord& log_record) {
    std::cout << log_record.thread_id << " " << log_record.timestamp << " [" << log_record.level << "] "
              << log_record.message << std::endl;
}

 void Logger::add_sink(std::unique_ptr<Sink> sink) {
    this->sinks.push_back(std::move(sink));
 }

void Logger::log(std::thread::id thread_id, const std::string& message, LogLevel level) {
    LogRecord logRecord(thread_id, message, level);

    for (const auto& sink : sinks) {
        sink->log(logRecord);
    }
}

void Logger::INFO(std::thread::id thread_id, const std::string&message) {
    this->log(thread_id, message, LogLevel::INFO);
}

void Logger::DEBUG(std::thread::id thread_id, const std::string&message) {
    this->log(thread_id, message, LogLevel::DEBUG);
}

void Logger::WARN(std::thread::id thread_id, const std::string&message) {
    this->log(thread_id, message, LogLevel::WARN);
}

void Logger::ERROR(std::thread::id thread_id, const std::string&message) {
    this->log(thread_id, message, LogLevel::ERROR);
}
