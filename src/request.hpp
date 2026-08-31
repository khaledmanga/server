#pragma once

#include <cstdint>
#include <string>

enum class RequestState { RequestLine, Header, Body, Complete };

struct RequestLine {
  std::string method;
  std::string path;
  uint8_t major_version;
  uint8_t minor_version;
};

struct Header {
  std::string host;
  std::string user_agent;
  std::string accept;
  std::string content_type;
  int content_length = 0;
};

struct Request {
  RequestLine request_line;
  Header header;
  std::string body;

  void print_request_line() const;
  void print_header() const;
  void print_body() const;
};

void parse_request_line(Request &request, const std::string &request_line_raw);
void parse_header(Request &request, const std::string &request_header_raw);
bool parse_request(Request &request, RequestState &state,
                   std::string &request_raw);
