#pragma once

#include <iostream>
#include <string>

enum class HttpRequestState {
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
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  BAD_GATEWAY = 502
};

std::string getReasonPhrase(StatusCode code);
} // namespace HTTP

namespace Log {
enum class Level { DEBUG, INFO, WARN, ERROR };
}