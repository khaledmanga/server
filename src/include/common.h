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

#endif