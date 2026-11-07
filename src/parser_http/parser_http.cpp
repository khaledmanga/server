#include "parser_http.hpp"

#include <sstream>
#include <string>

bool parse_request_line(const std::string &line, Request &request) {
  std::istringstream iss(line);
  std::string method, path, version;

  if (!(iss >> method >> path >> version)) {
    return false;
  }

  request.request_line.method = method;
  request.request_line.path = path;
  request.method = method;
  request.uri = path;

  size_t slash_pos = version.find('/');
  if (slash_pos == std::string::npos) {
    return false;
  }

  request.request_line.protocol = version.substr(0, slash_pos);
  request.protocol = request.request_line.protocol;

  size_t dot_pos = version.find('.', slash_pos + 1);
  if (dot_pos == std::string::npos) {
    return false;
  }

  request.request_line.major_version = std::stoi(version.substr(slash_pos + 1, dot_pos - slash_pos - 1));
  request.request_line.minor_version = std::stoi(version.substr(dot_pos + 1));
  request.major_version = request.request_line.major_version;
  request.minor_version = request.request_line.minor_version;

  return true;
}

bool parse_header_line(const std::string &line, Request &request) {
  size_t start = 0;
  while (start < line.size()) {
    size_t end = line.find("\r\n", start);
    size_t length = end == std::string::npos ? std::string::npos : end - start;
    std::string header_line = line.substr(start, length);

    if (header_line.empty()) {
      return false;
    }

    size_t colon_pos = header_line.find(':');
    if (colon_pos == std::string::npos) {
      return false;
    }

    std::string key = header_line.substr(0, colon_pos);
    std::string value = header_line.substr(colon_pos + 1);

    if (!value.empty() && value.front() == ' ') {
      value.erase(0, 1);
    }

    if (key == "Content-Length") {
      request.header.content_length = value;
      request.content_length = value;
    } else if (key == "Content-Type") {
      request.header.content_type = value;
      request.content_type = value;
    } else if (key == "Host") {
      request.header.host = value;
      request.host = value;
    }

    if (end == std::string::npos) {
      break;
    }
    start = end + 2;
  }

  return true;
}

bool parse_body(const std::string &body, Request &request) {
  if (request.header.content_length.empty()) {
    request.body.content.clear();
    return true;
  }

  size_t body_length = std::stoul(request.header.content_length);
  request.body.content = body.substr(0, body_length);
  return true;
}

void parse_http_request(HttpParserState &parser_state, const std::string &raw_request, Request &request) {
  size_t CLRF_pos = raw_request.find("\r\n");

  if (CLRF_pos == std::string::npos) {
    parser_state = HttpParserState::ERROR;
    return;
  }

  if (!parse_request_line(raw_request.substr(0, CLRF_pos), request)) {
    parser_state = HttpParserState::ERROR;
    return;
  }

  std::string remaining = raw_request.substr(CLRF_pos + 2);
  parser_state = HttpParserState::HEADERS;

  size_t header_end_pos = remaining.find("\r\n\r\n");
  if (header_end_pos == std::string::npos) {
    parser_state = HttpParserState::ERROR;
    return;
  }

  if (!parse_header_line(remaining.substr(0, header_end_pos), request)) {
    parser_state = HttpParserState::ERROR;
    return;
  }

  remaining = remaining.substr(header_end_pos + 4);
  parser_state = HttpParserState::BODY;

  if (!parse_body(remaining, request)) {
    parser_state = HttpParserState::ERROR;
    return;
  }

  parser_state = HttpParserState::COMPLETE;
}