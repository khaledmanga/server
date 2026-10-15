#pragma once

#include "include/common.h"
#include <optional>
#include <string>
#include <unordered_map>

class StatusLine {
public:
  std::string protocol;
  int major_version;
  int minor_version;
  int status_code;
  std::string message;
};

class ResponseHeader {
public:
  std::unordered_map<std::string, std::string> kv;

  std::optional<std::string> operator[](const std::string &key) const;
};

class ResponseBody {
public:
  std::string type;
  std::string value;
};

class Response {
private:
  std::string serialize(const StatusCode &status_code);

public:
  StatusLine status_line;
  ResponseHeader header;
  ResponseBody body;

  std::string ok();
  std::string not_found();
};