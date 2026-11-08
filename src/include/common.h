#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include "http/request/request.hpp"
#include "http/response/response.hpp"
#include <functional>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

using Handler = std::function<void(Request &req, Response &res)>;

enum class HTTP_METHOD {
  GET,
  POST,
  PUT,
  DELETE,
};

enum class LOG_LEVEL {
    DEBUG,
    INFO,
    ERROR,
    WARN,
};

inline std::string to_log_level_string(LOG_LEVEL log_level) {
    switch(log_level) {
        case LOG_LEVEL::DEBUG: return "DEBUG";
        case LOG_LEVEL::INFO: return "INFO";
        case LOG_LEVEL::ERROR: return "ERROR";
        case LOG_LEVEL::WARN: return "WARN";
    }
}

inline std::string to_string(HTTP_METHOD method) {
    switch (method) {
        case HTTP_METHOD::GET:    return "GET";
        case HTTP_METHOD::POST:   return "POST";
        case HTTP_METHOD::PUT:    return "PUT";
        case HTTP_METHOD::DELETE: return "DELETE";
        default:                  return "UNKNOWN";
    }
}

inline HTTP_METHOD from_string(const std::string& method_str) {
    if (method_str == "GET")    return HTTP_METHOD::GET;
    if (method_str == "POST")   return HTTP_METHOD::POST;
    if (method_str == "PUT")    return HTTP_METHOD::PUT;
    if (method_str == "DELETE") return HTTP_METHOD::DELETE;
}

inline std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    std::tm local_tm;
    localtime_r(&time_t_now, &local_tm);

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << millis;

    return oss.str();
}

#endif