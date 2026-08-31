#include "request.hpp"
#include <iostream>
#include <sstream>

void Request::print_request_line() const {
  std::cout << "Method: " << request_line.method << std::endl;
  std::cout << "Path: " << request_line.path << std::endl;
  std::cout << "Version: HTTP/" << static_cast<int>(request_line.major_version)
            << "." << static_cast<int>(request_line.minor_version) << std::endl;
}

void Request::print_header() const {
  std::cout << "Host: " << header.host << std::endl;
  std::cout << "User-Agent: " << header.user_agent << std::endl;
  std::cout << "Accept: " << header.accept << std::endl;
  std::cout << "Content-Type: " << header.content_type << std::endl;
  std::cout << "Content-Length: " << header.content_length << std::endl;
}

void Request::print_body() const { std::cout << "Body: " << body << std::endl; }

void parse_request_line(Request &request, const std::string &request_line_raw) {
  std::string method;
  std::string path;
  std::string version;
  std::istringstream ss(request_line_raw);

  ss >> method >> path >> version;

  size_t pos_slash = version.find("/");
  size_t pos_dot = version.find(".");

  if (pos_slash == std::string::npos || pos_dot == std::string::npos)
    return;

  request.request_line.method = method;
  request.request_line.path = path;

  request.request_line.major_version = static_cast<uint8_t>(
      std::stoi(version.substr(pos_slash + 1, pos_dot - pos_slash - 1)));
  request.request_line.minor_version =
      static_cast<uint8_t>(std::stoi(version.substr(pos_dot + 1)));
}

void parse_header(Request &request, const std::string &request_header_raw) {
  size_t pos_crlf = request_header_raw.find("\r\n");
  size_t start = 0;

  while (pos_crlf != std::string::npos) {
    std::string line = request_header_raw.substr(start, pos_crlf - start);
    size_t pos_colon = line.find(":");

    if (pos_colon != std::string::npos) {
      std::string key = line.substr(0, pos_colon);
      std::string value = line.substr(pos_colon + 1);

      if (!value.empty() && value[0] == ' ') {
        value.erase(0, 1);
      }

      if (key == "Host") {
        request.header.host = value;
      } else if (key == "User-Agent") {
        request.header.user_agent = value;
      } else if (key == "Accept") {
        request.header.accept = value;
      } else if (key == "Content-Type") {
        request.header.content_type = value;
      } else if (key == "Content-Length") {
        request.header.content_length = std::stoi(value);
      }
    }
    start = pos_crlf + 2;
    pos_crlf = request_header_raw.find("\r\n", start);
  }
}

bool parse_request(Request &request, RequestState &state,
                   std::string &request_raw) {

  while (true) {

    if (state == RequestState::RequestLine) {
      size_t pos = request_raw.find("\r\n");

      if (pos == std::string::npos)
        return false;

      std::string line = request_raw.substr(0, pos);

      parse_request_line(request, line);

      request_raw.erase(0, pos + 2);

      state = RequestState::Header;
    }

    else if (state == RequestState::Header) {
      size_t pos = request_raw.find("\r\n\r\n");

      if (pos == std::string::npos)
        return false;

      std::string headers = request_raw.substr(0, pos + 2);

      parse_header(request, headers);

      request_raw.erase(0, pos + 4);

      state = RequestState::Body;
    }

    else if (state == RequestState::Body) {
      size_t len = request.header.content_length;

      if (request_raw.size() < len)
        return false;

      request.body.assign(request_raw, 0, len);

      request_raw.erase(0, len);

      state = RequestState::Complete;
    }

    else if (state == RequestState::Complete) {
      return true;
    }
  }
}
