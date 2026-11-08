#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include "http/request/request.hpp"
#include "http/response/response.hpp"
#include <functional>

using Handler = std::function<void(Request &req, Response &res)>;

enum class HTTP_METHOD {
  GET,
  POST,
  PUT,
  DELETE,
};

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

#endif