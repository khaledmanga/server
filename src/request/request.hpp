#pragma once

#include <cstring>
#include <iostream>

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
  std::string host;
  std::string content_type;
  int content_length = 0;

  friend std::ostream &operator<<(std::ostream &os, const Header &h);
};

class Body {
 public:
  std::string value;

  friend std::ostream &operator<<(std::ostream &os, const Body &b);
};

class Request {
 public:
  RequestLine request_line;
  Header header;
  Body body;

  friend std::ostream &operator<<(std::ostream &os, const Request &r);
};
