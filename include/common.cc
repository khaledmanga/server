#include "common.h"

int toStatusCode(StatusCode status_code) {
  switch (status_code) {
  case StatusCode::OK:
    return 200;
  case StatusCode::NotFound:
    return 404;
  }
  return 500;
}

std::string toStatusMessage(StatusCode status_code) {
  switch (status_code) {
  case StatusCode::OK:
    return "OK";
  case StatusCode::NotFound:
    return "Not found";
  }
  return "Internal Server Error";
}

std::string getCurrentTime() {
  auto now = std::chrono::system_clock::now();

  return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

std::ostream &operator<<(std::ostream &os, const LogLevel &level) {
  switch (level) {
  case LogLevel::INFO:
    os << "INFO";
    break;
  case LogLevel::DEBUG:
    os << "DEBUG";
    break;
  case LogLevel::ERROR:
    os << "ERROR";
    break;
  case LogLevel::WARN:
    os << "WARN";
    break;
  default:
    os << "UNKNOWN";
    break;
  }
  return os;
}