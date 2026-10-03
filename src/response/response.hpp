#pragma once

#include <string>

#include "../constant/common.hpp"

class Response {
 public:
  Response();

  Response &status(HTTP::StatusCode status_code);
  Response &send(const std::string &value);
  std::string serialize() const;

 private:
  HTTP::StatusCode status_code;
  std::string body;
};
