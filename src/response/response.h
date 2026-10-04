#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "../constant/common.h"

class Response {
 public:
  Response();

  Response &status(HTTP::StatusCode status_code);
  Response &send(const std::string &value);
  Response &json(const nlohmann::json &value);
  std::string serialize() const;

 private:
  HTTP::StatusCode status_code;
  std::string body;
  std::string content_type;
};
