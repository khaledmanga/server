#pragma once

#include <iostream>
#include <cstring>

class RequestLine {
  std::string method;
  std::string target;
  std::string protocol;
  int major_version;
  int minor_version;
};

class Header {
  std::string host;
  std::string content_type;
  int content_length;
}

class Body {
  
}

class Request {
  RequestLine request_line;
  Header header;
  Body body;
}
