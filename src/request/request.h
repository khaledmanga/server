#pragma once

#include <cstring>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

class RequestLine {
 public:
  std::string method;
  std::string target;
  std::string protocol;
  int major_version;
  int minor_version;

  friend std::ostream &operator<<(std::ostream &os, const RequestLine &rl);
};

class Header {
 public:
  std::unordered_map<std::string, std::string> fields;

  friend std::ostream &operator<<(std::ostream &os, const Header &h);
};

class Body {
 public:
  std::string value;
  std::string content_type;

  json operator[](const std::string &key);
  friend std::ostream &operator<<(std::ostream &os, const Body &b);
};

class Request {
 public:
  RequestLine request_line;
  Header header;
  Body body;
  std::unordered_map<std::string, std::string> path_params;

  friend std::ostream &operator<<(std::ostream &os, const Request &r);
};
