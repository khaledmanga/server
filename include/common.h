#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <string>

enum class HttpStatus {
  Start,
  RequestLine,
  Header,
  Body,
  End,
  Error,
};

enum class StatusCode {
  OK,
  NotFound,
};

enum class LogLevel {
  INFO,
  DEBUG,
  ERROR,
  WARN,
};

std::ostream &operator<<(std::ostream &os, const LogLevel &level);
int toStatusCode(StatusCode status_code);
std::string toStatusMessage(StatusCode status_code);
std::string getCurrentTime();