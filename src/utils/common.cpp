#include "common.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

bool isValidMethodHttp(const std::string &http_method) {
  if (http_method == to_string(HttpMethod::GET) ||
      http_method == to_string(HttpMethod::POST)) {
    return true;
  }

  return false;
}

std::string getCurrentTime() {
  const std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm localTime{};
  localtime_r(&now, &localTime);
  std::ostringstream timestamp;
  timestamp << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
  return timestamp.str();
}