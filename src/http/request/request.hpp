#ifndef HTTP_REQUEST_REQUEST_HPP
#define HTTP_REQUEST_REQUEST_HPP

#include <string>

struct RequestLine {
  std::string method;
  std::string path;
  std::string protocol;
  int major_version;
  int minor_version;
};

struct RequestHeader {
  std::string content_type;
  std::string content_length;
  std::string host;
};

struct RequestBody {
  std::string type;
  std::string content;
};

struct Request {
  RequestLine request_line;
  RequestHeader header;
  RequestBody body;

  std::string method;
  std::string uri;
  std::string protocol;
  int major_version = 0;
  int minor_version = 0;
  std::string content_type;
  std::string content_length;
  std::string host;
};

#endif