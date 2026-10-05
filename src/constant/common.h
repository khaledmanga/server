#pragma once

#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

enum class HttpRequestState {
  Init,
  RequestLine,
  Headers,
  Body,
  Completed,
};

enum class HttpMethod {
  GET,
  POST,
  DELETE,
  PATCH,
  PUT,
};

std::string to_string(HttpMethod method);

namespace HTTP {
enum class StatusCode : int {
  OK = 200,
  CREATED = 201,
  NO_CONTENT = 204,
  MOVED_PERMANENTLY = 301,
  FOUND = 302,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  NOT_ALLOWED = 405,
  REQUEST_TIMEOUT = 408,
  PAYLOAD_TOO_LARGE = 413,
  UNSUPPORTED_MEDIA_TYPE = 415,
  TOO_MANY_REQUESTS = 429,
  SERVICE_UNAVAILABLE = 503,
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  BAD_GATEWAY = 502
};

std::string getReasonPhrase(StatusCode code);
}  // namespace HTTP

namespace Log {
enum class Level { DEBUG, INFO, WARN, ERROR };
}

const std::string VALUE_EMPTY = "";